`define WHITE 2'd1
`define BLACK -2'd1
`define EMPTY 2'd0

module queen(input logic clk, input logic rst_n,
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

    enum {WAIT, RD_ARGS, RD_B1, RD_B2, 
        MV_F, ADD_FMV, EDIT_F, WRITE_SQF, 
        PREP_B, MV_B, ADD_BMV, EDIT_B, WRITE_SQB, 
        PREP_L, MV_L, ADD_LMV, EDIT_L, WRITE_SQL,
        PREP_R, MV_R, ADD_RMV, EDIT_R, WRITE_SQR,

        PREP_FR, MV_FR, ADD_FRMV, EDIT_FR, WRITE_SQFR, 
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
    logic [7:0] count, board_offset, sq_data, board_count, tmp_offset;

    logic done_direction; // set flag if taking enemy piece so we stop after adding it

    // return the number of potential moves generated to the CPU
    assign slave_readdata = board_count;

    assign master_write = state == WRITE_SQF || state == WRITE_SQB || state == WRITE_SQL || state == WRITE_SQR || state == WRITE_SQFR || state == WRITE_SQBR || state == WRITE_SQFL || state == WRITE_SQBL;
    assign master_read = state == RD_B1;

    // signal settings
    always @(posedge clk) begin
        if (~rst_n) begin
            slave_waitrequest = 1'd1;

            state = WAIT;
        end else begin
            case(state)
                WAIT: begin
                    slave_waitrequest = 1'd0;
                    count = 8'd0;
                    board_count = 8'd0;
                    done_direction = 1'd0;

                    state = slave_write ? RD_ARGS : WAIT; // wait until cpu tries to write to addr0 (meaning they are done writing params)
                end
                
                // if cpu is writing, receive and save params
                RD_ARGS: begin
                    slave_waitrequest = 1'd1;
                    case(slave_address)
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

                    state = slave_address == 4'd0 ? RD_B1 : WAIT;
                end

                // set up read from SDRAM at src to get square (x, y)
                RD_B1: begin
                    slave_waitrequest = 1'd1;
                    if (count < 8'd64) begin
                        master_address = src + (count << 2'd2); // SHIFT OFFSET!!!
                    end

                    state = master_waitrequest ? RD_B1 : RD_B2;
                end

                // get board from mem
                RD_B2: begin
                    src_board[(master_address - src) >> 2'd2] = master_readdata[7:0];

                    // if this is the current piece, save it + its colour
                    if (master_address == (src + ((y << 2'd3) + x) << 2'd2)) begin
                        colour = signed'(master_readdata) > signed'(1'd0) ? `WHITE : (signed'(master_readdata) < signed'(1'd0) ? `BLACK : `EMPTY);
                        piece = master_readdata;
                    end
                    // only inc counter when we are done
                    if (master_readdatavalid) count = count + 1;
                    
                    state = master_readdatavalid & (count > 8'd63) ? MV_F : (master_readdatavalid ? RD_B1 : RD_B2);
                end

                // move forward one square
                MV_F: begin
                    tmp_y = tmp_y + 1'd1;
                    tmp_offset = (tmp_y << 2'd3) + tmp_x;
                    tmp_colour = signed'(src_board[tmp_offset][7:0]) > signed'(8'd0) ? `WHITE : (signed'(src_board[tmp_offset][7:0]) < signed'(8'd0) ? `BLACK : `EMPTY);

                    state = ADD_FMV;
                end

                // decide whether or not to add mv: do it if square is empty or has enemy piece
                ADD_FMV: begin
                    // if taking enemy or empty square, prep to write move
                    if (signed'(tmp_y) <= signed'(8'd7) & (tmp_colour != colour | tmp_colour == `EMPTY)) begin
                        count = 8'd0;
                        board_offset = 8'd0;
                        board_count = board_count + 1'd1;
                        done_direction = tmp_colour != `EMPTY;
                    end

                    state = (tmp_colour == colour) | (signed'(tmp_y) > signed'(8'd7)) ? PREP_B : EDIT_F; // if sq had friendly piece (or not on board), done in this direction
                end

                // decide what goes in the square
                EDIT_F: begin
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

                    state = (count > 8'd63) & done_direction ? PREP_B : (count > 8'd63 ? MV_F : WRITE_SQF);
                end

                // write the square to mem
                WRITE_SQF: begin
                    master_address = dest + ((board_offset + ((board_count - 1) << 3'd6)) << 2'd2); // need board count - 1 bc big line of boards, and inc count b4 writing
                    master_writedata = sq_data;

                    if (~master_waitrequest) count = count + 1'd1;

                    state = master_waitrequest ? WRITE_SQF : EDIT_F;
                end

                // set up going backwards
                PREP_B: begin
                    tmp_x = x;
                    tmp_y = y;
                    done_direction = 1'd0;

                    state = MV_B;
                end

                // move backward one square
                MV_B: begin
                    tmp_y = tmp_y - 1'd1;
                    tmp_colour = signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) > signed'(1'd0) ? `WHITE : (signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) < signed'(1'd0) ? `BLACK : `EMPTY);

                    state = ADD_BMV;
                end

                // decide whether or not to add mv: do it if square is empty or has enemy piece
                ADD_BMV: begin
                    // if taking enemy or empty square, prep to write move
                    if (signed'(tmp_y) >= signed'(8'd0) & (tmp_colour != colour || tmp_colour == `EMPTY)) begin
                        count = 8'd0;
                        board_offset = 8'd0;
                        board_count = board_count + 1'd1;
                        done_direction = tmp_colour != `EMPTY;
                    end

                    state = (tmp_colour == colour) | (signed'(tmp_y) < signed'(8'd0)) ? PREP_L : EDIT_B; // if sq had friendly piece, done in this direction
                end

                // decide what goes in the square
                EDIT_B: begin
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

                    state = (count > 8'd63) & done_direction ? PREP_L : (count > 8'd63 ? MV_B : WRITE_SQB);
                end

                // write the square to mem
                WRITE_SQB: begin
                    master_address = dest + ((board_offset + ((board_count - 1) << 3'd6)) << 2'd2); // need board count - 1 bc big line of boards, and inc count b4 writing
                    master_writedata = sq_data;

                    if (~master_waitrequest) count = count + 1'd1;

                    state = master_waitrequest ? WRITE_SQB : EDIT_B;
                end

                // set up going left
                PREP_L: begin
                    tmp_x = x;
                    tmp_y = y;
                    done_direction = 1'd0;

                    state = MV_L;
                end

                // move left one square
                MV_L: begin
                    tmp_x = tmp_x - 1'd1;
                    tmp_colour = signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) > signed'(1'd0) ? `WHITE : (signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) < signed'(1'd0) ? `BLACK : `EMPTY);

                    state = ADD_LMV;
                end

                // decide whether or not to add mv: do it if square is empty or has enemy piece
                ADD_LMV: begin
                    // if taking enemy or empty square, prep to write move
                    if (signed'(tmp_x) >= signed'(8'd0) & (tmp_colour != colour || tmp_colour == `EMPTY)) begin
                        count = 8'd0;
                        board_offset = 8'd0;
                        board_count = board_count + 1'd1;
                        done_direction = tmp_colour != `EMPTY;
                    end

                    state = (tmp_colour == colour) | (signed'(tmp_x) < signed'(8'd0)) ? PREP_R : EDIT_L; // if sq had friendly piece, done in this direction
                end

                // decide what goes in the square
                EDIT_L: begin
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

                    state = (count > 8'd63) & done_direction ? PREP_R : (count > 8'd63 ? MV_L : WRITE_SQL);
                end

                // write the square to mem
                WRITE_SQL: begin
                    master_address = dest + ((board_offset + ((board_count - 1) << 3'd6)) << 2'd2); // need board count - 1 bc big line of boards, and inc count b4 writing
                    master_writedata = sq_data;

                    if (~master_waitrequest) count = count + 1'd1;

                    state = master_waitrequest ? WRITE_SQL : EDIT_L;
                end

                // set up going right
                PREP_R: begin
                    tmp_x = x;
                    tmp_y = y;
                    done_direction = 1'd0;

                    state = MV_R; // set up correct x, y for going right
                end

                // move right one square
                MV_R: begin
                    tmp_x = tmp_x + 1'd1;
                    tmp_colour = signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) > signed'(1'd0) ? `WHITE : (signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) < signed'(1'd0) ? `BLACK : `EMPTY);

                    state = ADD_RMV;
                end

                // decide whether or not to add mv: do it if square is empty or has enemy piece
                ADD_RMV: begin
                    // if taking enemy or empty square, prep to write move
                    if (signed'(tmp_x) <= signed'(8'd7) & (tmp_colour != colour || tmp_colour == `EMPTY)) begin
                        count = 8'd0;
                        board_offset = 8'd0;
                        board_count = board_count + 1'd1;
                        done_direction = tmp_colour != `EMPTY;
                    end

                    state = (tmp_colour == colour) | (signed'(tmp_x) > signed'(8'd7)) ? PREP_FR : EDIT_R; // if sq had friendly piece, done in this direction
                end

                // decide what goes in the square
                EDIT_R: begin
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

                    state = (count > 8'd63) & done_direction ? PREP_FR : (count > 8'd63 ? MV_R : WRITE_SQR);
                end

                // write the square to mem
                WRITE_SQR: begin
                    master_address = dest + ((board_offset + ((board_count - 1) << 3'd6)) << 2'd2); // need board count - 1 bc big line of boards, and inc count b4 writing
                    master_writedata = sq_data;

                    if (~master_waitrequest) count = count + 1'd1;

                    state = master_waitrequest ? WRITE_SQR : EDIT_R;
                end

                // set up going right/forward
                PREP_FR: begin
                    tmp_x = x;
                    tmp_y = y;
                    done_direction = 1'd0;

                    state = MV_FR;
                end

                // move forward/right one square
                MV_FR: begin
                    tmp_x = tmp_x + 1'd1;
                    tmp_y = tmp_y + 1'd1;
                    tmp_offset = (tmp_y << 2'd3) + tmp_x;
                    tmp_colour = signed'(src_board[tmp_offset][7:0]) > signed'(8'd0) ? `WHITE : (signed'(src_board[tmp_offset][7:0]) < signed'(8'd0) ? `BLACK : `EMPTY);

                    state = ADD_FRMV;
                end

                // decide whether or not to add mv: do it if square is empty or has enemy piece
                ADD_FRMV: begin
                    // if taking enemy or empty square, prep to write move
                    if ((signed'(tmp_y) <= signed'(8'd7)) & (signed'(tmp_x) <= signed'(8'd7)) & (tmp_colour != colour | tmp_colour == `EMPTY)) begin
                        count = 8'd0;
                        board_offset = 8'd0;
                        board_count = board_count + 1'd1;
                        done_direction = tmp_colour != `EMPTY;
                    end

                    state = (tmp_colour == colour) | (signed'(tmp_y) > signed'(8'd7)) | (signed'(tmp_x) > signed'(8'd7)) ? PREP_BR : EDIT_FR; // if sq had friendly piece (or not on board), done in this direction
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

                    state = (count > 8'd63) & done_direction ? PREP_BR : (count > 8'd63 ? MV_FR : WRITE_SQFR);
                end

                // write the square to mem
                WRITE_SQFR: begin
                    master_address = dest + ((board_offset + ((board_count - 1) << 3'd6)) << 2'd2); // need board count - 1 bc big line of boards, and inc count b4 writing
                    master_writedata = sq_data;

                    if (~master_waitrequest) count = count + 1'd1;

                    state = master_waitrequest ? WRITE_SQFR : EDIT_FR;
                end

                // set up going backwards/right
                PREP_BR: begin
                    tmp_x = x;
                    tmp_y = y;
                    done_direction = 1'd0;

                    state = MV_BR;
                end

                // move backward/right one square
                MV_BR: begin
                    tmp_x = tmp_x + 1'd1;
                    tmp_y = tmp_y - 1'd1;
                    tmp_colour = signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) > signed'(1'd0) ? `WHITE : (signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) < signed'(1'd0) ? `BLACK : `EMPTY);

                    state = ADD_BRMV;
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

                    state = (tmp_colour == colour) | (signed'(tmp_y) < signed'(8'd0)) | (signed'(tmp_x) > signed'(8'd7)) ? PREP_FL : EDIT_BR; // if sq had friendly piece, done in this direction
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

                    state = (count > 8'd63) & done_direction ? PREP_FL : (count > 8'd63 ? MV_BR : WRITE_SQBR);
                end

                // write the square to mem
                WRITE_SQBR: begin
                    master_address = dest + ((board_offset + ((board_count - 1) << 3'd6)) << 2'd2); // need board count - 1 bc big line of boards, and inc count b4 writing
                    master_writedata = sq_data;

                    if (~master_waitrequest) count = count + 1'd1;

                    state = master_waitrequest ? WRITE_SQBR : EDIT_BR;
                end

                // set up going left/forward
                PREP_FL: begin
                    tmp_x = x;
                    tmp_y = y;
                    done_direction = 1'd0;

                    state = MV_FL;
                end

                // move forward/left one square
                MV_FL: begin
                    tmp_x = tmp_x - 1'd1;
                    tmp_y = tmp_y + 1'd1;
                    tmp_colour = signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) > signed'(1'd0) ? `WHITE : (signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) < signed'(1'd0) ? `BLACK : `EMPTY);

                    state = ADD_FLMV;
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

                    state = (tmp_colour == colour) | (signed'(tmp_x) < signed'(8'd0)) | (signed'(tmp_y) > signed'(8'd7)) ? PREP_BL : EDIT_FL; // if sq had friendly piece, done in this direction
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

                    state = (count > 8'd63) & done_direction ? PREP_BL : (count > 8'd63 ? MV_FL : WRITE_SQFL);
                end

                // write the square to mem
                WRITE_SQFL: begin
                    master_address = dest + ((board_offset + ((board_count - 1) << 3'd6)) << 2'd2); // need board count - 1 bc big line of boards, and inc count b4 writing
                    master_writedata = sq_data;

                    if (~master_waitrequest) count = count + 1'd1;

                    state = master_waitrequest ? WRITE_SQFL : EDIT_FL;
                end

                // set up going left/backwards
                PREP_BL: begin
                    tmp_x = x;
                    tmp_y = y;
                    done_direction = 1'd0;

                    state = MV_BL;
                end

                // move left/backwards one square
                MV_BL: begin
                    tmp_x = tmp_x - 1'd1;
                    tmp_y = tmp_y - 1'd1;
                    tmp_colour = signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) > signed'(1'd0) ? `WHITE : (signed'(src_board[(tmp_y << 2'd3) + tmp_x][7:0]) < signed'(1'd0) ? `BLACK : `EMPTY);

                    state = ADD_BLMV;
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

                    state = (tmp_colour == colour) | (signed'(tmp_x) < signed'(8'd0)) | (signed'(tmp_y) < signed'(8'd0)) ? FINISH : EDIT_BL; // if sq had friendly piece, done in this direction
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

                    state = (count > 8'd63) & done_direction ? FINISH : (count > 8'd63 ? MV_BL : WRITE_SQBL);
                end

                // write the square to mem
                WRITE_SQBL: begin
                    master_address = dest + ((board_offset + ((board_count - 1) << 3'd6)) << 2'd2); // need board count - 1 bc big line of boards, and inc count b4 writing
                    master_writedata = sq_data;

                    if (~master_waitrequest) count = count + 1'd1;

                    state = master_waitrequest ? WRITE_SQBL : EDIT_BL;
                end

                // allow read from addr 0
                FINISH: begin
                    slave_waitrequest = 1'd0;

                    state = slave_read ? WAIT : FINISH; // allow read (once requested) and go wait for next request
                end

            endcase
        end
    end

endmodule
