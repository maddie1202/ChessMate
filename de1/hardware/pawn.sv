`define NUM_MOVES_PAWN 4
`define EMPTY_PAWN 8'sd0

module pawn(input logic clk, input logic rst_n,
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

    enum { WAIT, INPUT, RD_SRC_PC, SV_SRC_PC, COMP_DEST_XYS, CHECK_DEST_XYS, 
        CHECK_DEST_ADDR, RD_DEST_PC, SV_DEST_PC, INC_CURR_MOVE, CHECK_DEST_PCS, 
        CHECK_BOARD, RD_SRC, SV_SRC, WR_DEST, INC_COPY_XY, INC_CURR_BOARD, FINISH } state;

    logic [`NUM_MOVES_PAWN * 8 - 1: 0] x_offsets, y_offsets, dest_xs, dest_ys, dest_pcs;
    logic [`NUM_MOVES_PAWN - 1: 0] move_valid;

    logic [31:0] src_board_addr, dest_board_addr, curr_dest_board_addr;
    logic [7:0] src_x, src_y, copy_x, copy_y, home_row;
    logic signed [7:0] src_pc, forward, copy_pc, check_val, check_val_x, check_val_y;
    logic src_is_white;
    integer curr_move, curr_board, move_i, move_j;

    assign src_is_white = src_pc >= 8'sd0;
    assign home_row = src_is_white ? 8'd1 : 8'd6;
    assign master_read = state == RD_SRC_PC || state == RD_DEST_PC || state == RD_SRC;
    assign master_write = state == WR_DEST;

    // slave_readdata
    always@(*) begin
        slave_readdata = 32'd0;
        for(move_j = 0; move_j < `NUM_MOVES_PAWN; move_j++) begin
            slave_readdata += move_valid[move_j];
        end
    end
    
    // slave_waitrequest
    always@ (posedge clk) begin
        if (~rst_n) begin
            slave_waitrequest = 0;
        end else begin
            case (state)
                WAIT: slave_waitrequest = 0;
                INPUT: slave_waitrequest = 1;
                RD_SRC_PC: slave_waitrequest = 1;
                FINISH: slave_waitrequest = 0;
            endcase
        end
    end

    // master_writedata
    always@ (*) begin
        if (state != WR_DEST) begin
            master_writedata = 32'hFFFFFFFF;
        end else if (copy_x === dest_xs[curr_board * 8 +: 8] && 
            copy_y === dest_ys[curr_board * 8 +: 8]) begin
            master_writedata = src_pc;
        end else if (copy_x === src_x && copy_y === src_y) begin
            master_writedata = `EMPTY_PAWN;
        end else begin
            master_writedata = copy_pc;
        end
    end

    // master_address
    always@ (*) begin
        case (state)
            RD_SRC_PC: master_address = src_board_addr + 32'd4 * ((src_y * 8) + src_x);
            RD_DEST_PC: master_address = src_board_addr + 32'd4 *  ((dest_ys[curr_move * 8 +: 8] * 8) + dest_xs[curr_move * 8 +: 8]);
            RD_SRC: master_address = src_board_addr + 32'd4 * ((copy_y * 8) + copy_x);
            WR_DEST: master_address = curr_dest_board_addr + 32'd4 * ((copy_y * 8) + copy_x);
            default: master_address = 32'hFFFFFFFF;
        endcase
    end

    // copy_x, copy_y
    always@ (posedge clk) begin
        if (~rst_n) begin
            copy_x = 8'd0;
            copy_y = 8'd0;
        end else if (state == CHECK_BOARD) begin
            copy_x = 8'd0;
            copy_y = 8'd0;
        end else if (state == INC_COPY_XY) begin
            if (copy_x == 8'd7) begin
                copy_x = 8'd0;
                copy_y++;
            end else begin
                copy_x++;
            end
        end
    end

    // curr_board, curr_dest_board_addr
    always@ (posedge clk) begin
        if (~rst_n || state == WAIT) begin
            curr_board = 0;
            curr_dest_board_addr = 32'd0;
        end else if (state == INC_CURR_BOARD) begin
            curr_dest_board_addr = curr_dest_board_addr + 32'd64 * 32'd4;
            curr_board++;
        end else if (state == INC_CURR_BOARD || 
            (state == CHECK_BOARD && ~move_valid[curr_board])) begin
            curr_board++;
        end else if (state == CHECK_DEST_PCS) begin
            curr_dest_board_addr = dest_board_addr;
        end
    end

    // curr_move
    always@ (posedge clk) begin
        if (~rst_n || state == WAIT) begin
            curr_move = 0;
        end else if (state == INC_CURR_MOVE || 
            (state == CHECK_DEST_ADDR && ~move_valid[curr_move])) begin
            curr_move++;
        end
    end

    // src_pc
    always@ (posedge clk) begin
        if (~rst_n) begin
            src_pc = 8'sd0;
        end else if (state == SV_SRC_PC) begin
            src_pc = master_readdata[7:0];
        end
    end

    // copy_pc
    always@ (posedge clk) begin
        if (~rst_n) begin
            copy_pc = 8'sd0;
        end else if (state == SV_SRC) begin
            copy_pc = master_readdata[7:0];
        end
    end

    // x_offsets,  y_offsets, dest_xs, dest_ys, move_valid, forward
    always@ (posedge clk) begin
        if (~rst_n || state == WAIT) begin
            x_offsets = {`NUM_MOVES_PAWN{8'hFF}};
            y_offsets = {`NUM_MOVES_PAWN{8'hFF}};
            dest_xs = {`NUM_MOVES_PAWN{8'hFF}};
            dest_ys = {`NUM_MOVES_PAWN{8'hFF}};
            move_valid = {`NUM_MOVES_PAWN{1'd1}};
            forward = 8'sd0;
        end else if (state == COMP_DEST_XYS) begin
            forward = src_is_white ? 8'sd1 : -8'sd1;
            x_offsets = {-8'sd1, 8'sd1, 8'sd0, 8'sd0};
            y_offsets = {forward, forward, forward * 8'sd2, forward};
            
            for (move_i = 0; move_i < `NUM_MOVES_PAWN; move_i++) begin
                dest_xs[move_i * 8 +: 8] = src_x + x_offsets[move_i * 8 +: 8];
            end

            for (move_i = 0; move_i < `NUM_MOVES_PAWN; move_i++) begin
                dest_ys[move_i * 8 +: 8] = src_y + y_offsets[move_i * 8 +: 8];
            end
        end else if (state == CHECK_DEST_XYS) begin
            for (move_i = 0; move_i < `NUM_MOVES_PAWN; move_i++) begin
                check_val_x = dest_xs[move_i * 8 +: 8];
                check_val_y = dest_ys[move_i * 8 +: 8];
                move_valid[move_i] = check_val_x >= 8'sd0 &&  
                    check_val_y >= 8'sd0 && check_val_x < 8'sd8 && check_val_y < 8'sd8;;
            end

            if (src_y !== home_row) begin
                move_valid[1] = 0;
            end
        end else if (state == CHECK_DEST_PCS) begin
            for (move_i = 0; move_i < `NUM_MOVES_PAWN; move_i++) begin
                check_val = dest_pcs[move_i * 8 +: 8];
                if (move_i > 1) begin
                    if (~((check_val >= 8'sd0 && src_pc < 8'sd0) || 
                        (check_val <= 8'sd0 && src_pc > 8'sd0))) move_valid[move_i] = 0;
                end else begin            
                    if (check_val !== `EMPTY_PAWN) move_valid[move_i] = 0;        
                end
            end
        end
    end

    // dest_pcs
    always@ (posedge clk) begin
        if (~rst_n) begin
            dest_pcs = {`NUM_MOVES_PAWN{8'HFF}};
        end else if (state == SV_DEST_PC) begin
            dest_pcs[curr_move * 8 +: 8] = master_readdata;
        end
    end

    // src_board_addr, dest_board_addr, src_x and src_y registers
    always@ (posedge clk) begin
        if (~rst_n) begin
            src_board_addr = 32'hFFFFFFFF;
            dest_board_addr = 32'hFFFFFFFF;
            src_x = 8'hFF;
            src_y = 8'hFF;
        end else if (state == INPUT) begin
            case (slave_address)
                4'd1: src_board_addr = slave_writedata;
                4'd2: dest_board_addr = slave_writedata;
                4'd3: src_x = slave_writedata[7:0];
                4'd4: src_y = slave_writedata[7:0];
            endcase
        end
    end

    // state 
    always @(posedge clk) begin
        if (~rst_n) begin
            state = WAIT;
        end else begin
            case (state)
                WAIT: begin
                    // if (slave_write && slave_address == 4'd0) begin
                    //     state = RD_SRC_PC;
                    // end else 
                    if (slave_write) begin
                        state = INPUT;
                    end else begin
                        state = WAIT;
                    end
                end
                INPUT: state = slave_address == 4'd0 ? RD_SRC_PC : WAIT;
                RD_SRC_PC: state = ~master_waitrequest ? SV_SRC_PC : RD_SRC_PC;
                SV_SRC_PC: state = master_readdatavalid ? COMP_DEST_XYS : SV_SRC_PC;
                COMP_DEST_XYS: state = CHECK_DEST_XYS;
                CHECK_DEST_XYS: state = CHECK_DEST_ADDR;
                CHECK_DEST_ADDR: state = move_valid[curr_move] ? RD_DEST_PC : CHECK_DEST_ADDR;
                RD_DEST_PC: state = ~master_waitrequest ? SV_DEST_PC : RD_DEST_PC;
                SV_DEST_PC: state = master_readdatavalid ? (curr_move < `NUM_MOVES_PAWN - 1 ? INC_CURR_MOVE : CHECK_DEST_PCS) : SV_DEST_PC;
                INC_CURR_MOVE: state = CHECK_DEST_ADDR;
                CHECK_DEST_PCS: state = CHECK_BOARD;
                CHECK_BOARD: state = curr_board == `NUM_MOVES_PAWN ? FINISH : move_valid[curr_board] ? RD_SRC : CHECK_BOARD;
                RD_SRC: state = ~master_waitrequest ? SV_SRC : RD_SRC;
                SV_SRC: state = master_readdatavalid ? WR_DEST : SV_SRC;
                WR_DEST: state = master_waitrequest ? WR_DEST : copy_x == 8'd7 && copy_y == 8'd7 ? INC_CURR_BOARD : INC_COPY_XY;
                INC_COPY_XY: state = RD_SRC;
                INC_CURR_BOARD: state = CHECK_BOARD;
                FINISH: state = slave_read && slave_address == 4'd0 ? WAIT : FINISH;
            endcase
        end
    end

endmodule