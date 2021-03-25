`define WHITE 2'd1
`define BLACK -2'd1
`define EMPTY 2'd0

module bishop(input logic clk, input logic rst_n, // STILL ROOK
    // slave (CPU-facing)
    output logic slave_waitrequest,
    input logic [3:0] slave_address,
    input logic slave_read, output logic [31:0] slave_readdata,
    input logic slave_write, input logic [31:0] slave_writedata,
    // master (SDRAM-facing)
    input logic master_waitrequest,
    output logic [31:0] master_address,
    output logic master_read, input logic [31:0] master_readdata, 
    input logic master_readdatavalid,
    output logic master_write, output logic [31:0] master_writedata);

    enum {RD_ARGS, RD_B1, RD_B2, 
        MV_FR, ADD_FRMV, EDIT_FR, WRITE_SQFR, 
        PREP_BR, MV_BR, ADD_BRMV, EDIT_BR, WRITE_SQBR, 
        PREP_FL, MV_FL, ADD_FLMV, EDIT_FL, WRITE_SQFL,
        PREP_BL, MV_BL, ADD_BLMV, EDIT_BL, WRITE_SQBL,
        FINISH} state;

    // mem addresses
    logic [31:0] src, dest;

    // current board
    logic signed [7:0] src_board [63:0];

    // signals for the piece's position
    logic [7:0] x, y; // [0, 7]
    logic signed [1:0] colour;
    logic signed [7:0] piece; // save what piece it is

    // temporary signals as we move around the board
    logic signed [7:0] tmp_x, tmp_y; // note that these could be negative since we set them first and check bounds later: [-1, 8]
    logic signed [1:0] tmp_colour;
    logic [7:0] count, board_offset, sq_data, board_count;

    logic done_direction; // set flag if taking enemy piece so we stop after adding it

    // signal settings
    always @(posedge clk) begin
        if (~rst_n) begin
            master_read = 1'd0;
            master_write = 1'd0;
            slave_waitrequest = 1'd1;
        end else begin
            case(state)
                // if cpu is writing, receive and save params
                RD_ARGS: begin
                    count = 8'd0;
                    board_count = 8'd0;
                    done_direction = 1'd0;
                    if (slave_write) begin
                        slave_waitrequest = 1'd0;
                        case(slave_address)
                            4'd0: slave_waitrequest = 1'd1; // done taking args
                            4'd1: src = slave_writedata;
                            4'd2: dest = slave_writedata;
                            4'd3: begin
                                x = slave_writedata;
                                tmp_x = slave_writedata;
                            end
                            4'd4: begin
                                y = slave_writedata;
                                tmp_y = slave_writedata;
                            end
                            // don't need default, since we want to do nothing otherwise
                        endcase
                    end
                end

                // set up read from SDRAM at src to get square (x, y)
                RD_B1: begin
                    if (count < 8'd64) begin
                        master_read = 1'd1;
                        master_address = src + count;
                    end
                end

                // get board from mem
                RD_B2: begin
                    master_read = 1'd0;
                    src_board[master_address - src] = master_readdata;

                    // if this is the current piece, save it + its colour
                    if (master_address == (src + (y << 2'd3) + x)) begin
                        colour = signed'(master_readdata) > signed'(1'd0) ? `WHITE : (signed'(master_readdata) < signed'(1'd0) ? `BLACK : `EMPTY);
                        piece = master_readdata;
                    end
                    // only inc counter when we are done
                    if (master_readdatavalid) count = count + 1;
                    
                end

                // move forward/right one square
                MV_FR: begin
                    tmp_x = tmp_x + 1'd1;
                    tmp_y = tmp_y + 1'd1;
                    tmp_colour = signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) > signed'(8'd0) ? `WHITE : (signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) < signed'(8'd0) ? `BLACK : `EMPTY);
                end

                // decide whether or not to add mv: do it if square is empty or has enemy piece
                ADD_FRMV: begin
                    // if taking enemy or empty square, prep to write move
                    if (signed'(tmp_y) <= signed'(8'd7) & signed'(tmp_x) <= signed'(8'd7) &(tmp_colour != colour | tmp_colour == `EMPTY)) begin
                        count = 8'd0;
                        board_offset = 8'd0;
                        board_count = board_count + 1'd1;
                        done_direction = tmp_colour != `EMPTY;
                    end
                end

                // decide what goes in the square
                EDIT_FR: begin
                    board_offset = count;
                    // original spot on board
                    if (count == ((y << 2'd3) + x)) begin
                        sq_data = `EMPTY; 
                    end
                    // new spot
                    else if (count == ((tmp_y << 2'd3) + tmp_x)) begin
                        sq_data = piece;
                    end
                    else begin
                        sq_data = src_board[count]; 
                    end
                end

                // write the square to mem
                WRITE_SQFR: begin
                    master_write = 1'd1;
                    master_address = dest + board_offset + ((board_count - 1) << 3'd6); // need board count - 1 bc big line of boards, and inc count b4 writing
                    master_writedata = sq_data;

                    if (~master_waitrequest) count = count + 1'd1;
                end

                // set up going backwards/right
                PREP_BR: begin
                    tmp_x = x;
                    tmp_y = y;
                    done_direction = 1'd0;
                end

                // move backward/right one square
                MV_BR: begin
                    tmp_x = tmp_x + 1'd1;
                    tmp_y = tmp_y - 1'd1;
                    tmp_colour = signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) > signed'(1'd0) ? `WHITE : (signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) < signed'(1'd0) ? `BLACK : `EMPTY);
                end

                // decide whether or not to add mv: do it if square is empty or has enemy piece
                ADD_BRMV: begin
                    // if taking enemy or empty square, prep to write move
                    if (signed'(tmp_y) >= signed'(8'd0) & signed'(tmp_x) <= signed'(8'd7) & (tmp_colour != colour || tmp_colour == `EMPTY)) begin
                        count = 8'd0;
                        board_offset = 8'd0;
                        board_count = board_count + 1'd1;
                        done_direction = tmp_colour != `EMPTY;
                    end
                end

                // decide what goes in the square
                EDIT_BR: begin
                    board_offset = count;
                    // original spot on board
                    if (count == ((y << 2'd3) + x)) begin
                        sq_data = `EMPTY; 
                    end
                    // new spot
                    else if (count == ((tmp_y << 2'd3) + tmp_x)) begin
                        sq_data = piece;
                    end
                    else begin
                        sq_data = src_board[count]; 
                    end
                end

                // write the square to mem
                WRITE_SQBR: begin
                    master_write = 1'd1;
                    master_address = dest + board_offset + ((board_count - 1) << 3'd6); // need board count - 1 bc big line of boards, and inc count b4 writing
                    master_writedata = sq_data;

                    if (~master_waitrequest) count = count + 1'd1;
                end

                // set up going left/forward
                PREP_FL: begin
                    tmp_x = x;
                    tmp_y = y;
                    done_direction = 1'd0;
                end

                // move forward/left one square
                MV_FL: begin
                    tmp_x = tmp_x - 1'd1;
                    tmp_y = tmp_y + 1'd1;
                    tmp_colour = signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) > signed'(1'd0) ? `WHITE : (signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) < signed'(1'd0) ? `BLACK : `EMPTY);
                end

                // decide whether or not to add mv: do it if square is empty or has enemy piece
                ADD_FLMV: begin
                    // if taking enemy or empty square, prep to write move
                    if (signed'(tmp_x) >= signed'(8'd0) & signed'(tmp_y) <= signed'(8'd7) & (tmp_colour != colour || tmp_colour == `EMPTY)) begin
                        count = 8'd0;
                        board_offset = 8'd0;
                        board_count = board_count + 1'd1;
                        done_direction = tmp_colour != `EMPTY;
                    end
                end

                // decide what goes in the square
                EDIT_FL: begin
                    board_offset = count;
                    // original spot on board
                    if (count == ((y << 2'd3) + x)) begin
                        sq_data = `EMPTY; 
                    end
                    // new spot
                    else if (count == ((tmp_y << 2'd3) + tmp_x)) begin
                        sq_data = piece;
                    end
                    else begin
                        sq_data = src_board[count]; 
                    end
                end

                // write the square to mem
                WRITE_SQFL: begin
                    master_write = 1'd1;
                    master_address = dest + board_offset + ((board_count - 1) << 3'd6); // need board count - 1 bc big line of boards, and inc count b4 writing
                    master_writedata = sq_data;

                    if (~master_waitrequest) count = count + 1'd1;
                end

                // set up going left/backwards
                PREP_BL: begin
                    tmp_x = x;
                    tmp_y = y;
                    done_direction = 1'd0;
                end

                // move left/backwards one square
                MV_BL: begin
                    tmp_x = tmp_x - 1'd1;
                    tmp_y = tmp_y - 1'd1;
                    tmp_colour = signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) > signed'(1'd0) ? `WHITE : (signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) < signed'(1'd0) ? `BLACK : `EMPTY);
                end

                // decide whether or not to add mv: do it if square is empty or has enemy piece
                ADD_BLMV: begin
                    // if taking enemy or empty square, prep to write move
                    if (signed'(tmp_x) >= signed'(8'd0) & signed'(tmp_y) >= signed'(8'd0) & (tmp_colour != colour || tmp_colour == `EMPTY)) begin
                        count = 8'd0;
                        board_offset = 8'd0;
                        board_count = board_count + 1'd1;
                        done_direction = tmp_colour != `EMPTY;
                    end
                end

                // decide what goes in the square
                EDIT_BL: begin
                    board_offset = count;
                    // original spot on board
                    if (count == ((y << 2'd3) + x)) begin
                        sq_data = `EMPTY; 
                    end
                    // new spot
                    else if (count == ((tmp_y << 2'd3) + tmp_x)) begin
                        sq_data = piece;
                    end
                    else begin
                        sq_data = src_board[count]; 
                    end
                end

                // write the square to mem
                WRITE_SQBL: begin
                    master_write = 1'd1;
                    master_address = dest + board_offset + ((board_count - 1) << 3'd6); // need board count - 1 bc big line of boards, and inc count b4 writing
                    master_writedata = sq_data;

                    if (~master_waitrequest) count = count + 1'd1;
                end

                // allow read from addr 0
                FINISH: begin
                    slave_waitrequest = 1'd0;
                end

            endcase
        end
    end

    // state transitions
    always @(posedge clk) begin
        if (~rst_n) begin
            state = RD_ARGS;
        end else begin
            case (state)
                RD_ARGS: state = slave_write & slave_address == 4'd0 ? RD_B1 : RD_ARGS; // wait until cpu tries to write to addr0 (meaning they are done writing params)
                RD_B1: state = master_waitrequest ? RD_B1 : RD_B2;
                RD_B2: state = master_readdatavalid & count > 8'd63 ? MV_FR : (master_readdatavalid ? RD_B1 : RD_B2);

                MV_FR: state = ADD_FRMV;
                ADD_FRMV: state = tmp_colour == colour | tmp_y > 8'd7 ? PREP_BR : EDIT_FR; // if sq had friendly piece (or not on board), done in this direction
                EDIT_FR: state = count > 8'd63 & done_direction ? PREP_BR : (count > 8'd63 ? MV_FR : WRITE_SQFR);
                WRITE_SQFR: state = master_waitrequest ? WRITE_SQFR : EDIT_FR;

                PREP_BR: state = MV_BR; // set up correct x, y for going backwards/right
                MV_BR: state = ADD_BRMV;
                ADD_BRMV: state = tmp_colour == colour | signed'(tmp_y) < signed'(8'd0) ? PREP_FL : EDIT_BR; // if sq had friendly piece, done in this direction
                EDIT_BR: state = count > 8'd63 & done_direction ? PREP_FL : (count > 8'd63 ? MV_BR : WRITE_SQBR);
                WRITE_SQBR: state = master_waitrequest ? WRITE_SQBR : EDIT_BR;

                PREP_FL: state = MV_FL; // set up correct x, y for going forwards/left
                MV_FL: state = ADD_FLMV;
                ADD_FLMV: state = tmp_colour == colour | signed'(tmp_x) < signed'(8'd0) ? PREP_BL : EDIT_FL; // if sq had friendly piece, done in this direction
                EDIT_FL: state = count > 8'd63 & done_direction ? PREP_BL : (count > 8'd63 ? MV_FL : WRITE_SQFL);
                WRITE_SQFL: state = master_waitrequest ? WRITE_SQFL : EDIT_FL;

                PREP_BL: state = MV_BL; // set up correct x, y for going backwards/left
                MV_BL: state = ADD_BLMV;
                ADD_BLMV: state = tmp_colour == colour | tmp_x > 8'd7 ? FINISH : EDIT_BL; // if sq had friendly piece, done in this direction
                EDIT_BL: state = count > 8'd63 & done_direction ? FINISH : (count > 8'd63 ? MV_BL : WRITE_SQBL);
                WRITE_SQBL: state = master_waitrequest ? WRITE_SQBL : EDIT_BL;

                FINISH: state = RD_ARGS; // allow read and go wait for next request

            endcase
        end
    end

endmodule
