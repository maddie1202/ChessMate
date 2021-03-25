`define OUT_0F_BOUNDS 8'sd128

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

    enum { WAIT, INPUT,
        RD_SRC, SV_SRC, RD_1F, SV_1F, RD_2F, SV_2F, RD_1F1L, SV_1F1L, RD_1F1R, SV_1F1R, 
        INIT_WR, INIT_COPY, RD, WR,
        FINISH } state;

    assign slave_waitrequest = state != WAIT;

    logic [31:0] src_board_addr, dest_board_addr;
    logic [7:0] src_x, src_y, dest_x, dest_y;
    logic [31:0] src_xy_offset, dest_xy_offset;
    logic signed [7:0] src_pc, f1_pc, f2_pc, f1l1_pc, f1lr_pc;
    logic src_pc_white;
    logic signed [7:0] forward;
    logic dest_pc_in_bounds;

    assign src_xy_offset  = (src_y << 3) + src_x;
    assign dest_xy_offset  = (dest_y << 3) + dest_x;
    assign src_pc_white = src_pc >= 8'sd0;
    assign forward = src_pc_white ? 8'sd1 : -8'sd1;
    assign dest_pc_in_bounds = dest_x >= 8'sd0 && dest_x < 8'sd8 && dest_y >= 8'sd0 && dest_y < 8'sd8;

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
     // src_pc, 1f_pc, 2f_pc, 1f1l_pc, 1f1r_pc registers
    always@ (posedge clk) begin
        if (~rst_n) begin
            src_pc = `OUT_0F_BOUNDS;
            f1_pc = `OUT_0F_BOUNDS;
            f2_pc = `OUT_0F_BOUNDS;
            f1l1_pc = `OUT_0F_BOUNDS;
            f1lr_pc = `OUT_0F_BOUNDS;
        end else if (dest_pc_in_bounds) begin
            case (state)
                SV_SRC:  src_pc  = master_readdata;
                SV_1F:   f1_pc   = master_readdata;
                SV_2F:   f2_pc   = master_readdata;
                SV_1F1L: f1l1_pc = master_readdata;
                SV_1F1R: f1lr_pc = master_readdata;
            endcase
        end else begin
            case (state)
                SV_SRC:  src_pc  = master_readdata;
                SV_1F:   f1_pc   = `OUT_0F_BOUNDS;
                SV_2F:   f2_pc   = `OUT_0F_BOUNDS;
                SV_1F1L: f1l1_pc = `OUT_0F_BOUNDS;
                SV_1F1R: f1lr_pc = `OUT_0F_BOUNDS;
            endcase
        end
    end

    always@ (*) begin
        master_read = 0;
        master_address = 32'hFFFFFFFF;
        dest_x = 8'hFF;
        dest_y = 8'hFF;

        case (state) 
            WAIT: begin
                        end
            INPUT:      begin
                        end
            RD_SRC:     begin
                            master_read = 1;
                            master_address = src_board_addr + src_xy_offset;
                        end
            SV_SRC:     begin
                        end
            RD_1F:      begin
                            dest_x = src_x;
                            dest_y = src_y + forward;
                            if (dest_pc_in_bounds) begin
                                master_read = 1;
                                master_address = src_board_addr + dest_xy_offset;
                            end
                        end
            SV_1F:      begin
                            dest_x = src_x;
                            dest_y = src_y + forward;
                        end
            RD_2F:      begin
                            master_read = 1;
                            dest_x = src_x;
                            dest_y = src_y + (forward << 1); // forward << 1 == forward * 2
                            master_address = src_board_addr + dest_xy_offset;
                        end
            SV_2F:      begin
                            dest_x = src_x;
                            dest_y = src_y + (forward << 1); // forward << 1 == forward * 2
                        end
            RD_1F1L:    begin
                            master_read = 1;
                            dest_x = src_x - 8'sd1;
                            dest_y = src_y + forward;
                            master_address = src_board_addr + dest_xy_offset;
                        end
            SV_1F1L:    begin
                            dest_x = src_x - 8'sd1;
                            dest_y = src_y + forward;
                        end
            RD_1F1R:    begin
                            master_read = 1;
                            dest_x = src_x - 8'sd1;
                            dest_y = src_y + forward;
                            master_address = src_board_addr + dest_xy_offset;
                        end
            SV_1F1R:    begin
                            dest_x = src_x - 8'sd1;
                            dest_y = src_y + forward;
                        end
            FINISH:     begin
                            
                        end
        endcase
    end

    always @(posedge clk) begin
        if (~rst_n) begin
            state = WAIT;
        end else begin
            case (state)
                WAIT: begin
                                if (slave_write && slave_address == 4'd0) begin
                                    state = RD_SRC;
                                end else if (slave_write) begin
                                    state = INPUT;
                                end else begin
                                    state = WAIT;
                                end
                            end
                INPUT: state = WAIT;
                RD_SRC: state = master_readdatavalid ? SV_SRC : RD_SRC;
                SV_SRC: state = RD_1F;
                RD_1F: state = master_readdatavalid || ~dest_pc_in_bounds ? SV_1F : RD_1F;
                SV_1F: state = RD_2F;
                RD_2F: state = master_readdatavalid || ~dest_pc_in_bounds ? SV_2F : RD_2F;
                SV_2F: state = RD_1F1L;
                RD_1F1L: state = master_readdatavalid || ~dest_pc_in_bounds ? SV_1F1L : RD_1F1L;
                SV_1F1L: state = RD_1F1R;
                RD_1F1R: state = master_readdatavalid || ~dest_pc_in_bounds ? SV_1F1R : RD_1F1R;
                SV_1F1R: state = FINISH;
                FINISH: state = slave_read && slave_address == 4'd0 ? WAIT : FINISH;
            endcase
        end
    end

endmodule