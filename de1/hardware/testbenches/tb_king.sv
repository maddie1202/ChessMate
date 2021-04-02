`define NUM_KING_MOVES 8
`define MEM_SIZE (`NUM_KING_MOVES * 64)

`define WPAWN0  1
`define WPAWN1  2
`define WPAWN2  3
`define WPAWN3  4
`define WPAWN4  5
`define WPAWN5  6
`define WPAWN6  7
`define WPAWN7  8
`define WROOK0  9
`define WROOK1  10
`define WROOK2  11
`define WROOK3  12
`define WROOK4  13
`define WROOK5  14
`define WROOK6  15
`define WROOK7  16
`define WROOK8  17
`define WROOK9  18
`define WKNIGHT0  19
`define WKNIGHT1  20
`define WKNIGHT2  21
`define WKNIGHT3  22
`define WKNIGHT4  23
`define WKNIGHT5  24
`define WKNIGHT6  25
`define WKNIGHT7  26
`define WKNIGHT8  27
`define WKNIGHT9  28
`define WBISHOP0  29
`define WBISHOP1  30
`define WBISHOP2  31
`define WBISHOP3  32
`define WBISHOP4  33
`define WBISHOP5  34
`define WBISHOP6  35
`define WBISHOP7  36
`define WBISHOP8  37
`define WBISHOP9  38
`define WQUEEN0  39
`define WQUEEN1  40
`define WQUEEN2  41
`define WQUEEN3  42
`define WQUEEN4  43
`define WQUEEN5  44
`define WQUEEN6  45
`define WQUEEN7  46
`define WQUEEN8  47
`define WKING    48

`define BPAWN0  -1
`define BPAWN1  -2
`define BPAWN2  -3
`define BPAWN3  -4
`define BPAWN4  -5
`define BPAWN5  -6
`define BPAWN6  -7
`define BPAWN7  -8
`define BROOK0  -9
`define BROOK1  -10
`define BROOK2  -11
`define BROOK3  -12
`define BROOK4  -13
`define BROOK5  -14
`define BROOK6  -15
`define BROOK7  -16
`define BROOK8  -17
`define BROOK9  -18
`define BKNIGHT0  -19
`define BKNIGHT1  -20
`define BKNIGHT2  -21
`define BKNIGHT3  -22
`define BKNIGHT4  -23
`define BKNIGHT5  -24
`define BKNIGHT6  -25
`define BKNIGHT7  -26
`define BKNIGHT8  -27
`define BKNIGHT9  -28
`define BBISHOP0  -29
`define BBISHOP1  -30
`define BBISHOP2  -31
`define BBISHOP3  -32
`define BBISHOP4  -33
`define BBISHOP5  -34
`define BBISHOP6  -35
`define BBISHOP7  -36
`define BBISHOP8  -37
`define BBISHOP9  -38
`define BQUEEN0  -39
`define BQUEEN1  -40
`define BQUEEN2  -41
`define BQUEEN3  -42
`define BQUEEN4  -43
`define BQUEEN5  -44
`define BQUEEN6  -45
`define BQUEEN7  -46
`define BQUEEN8  -47
`define BKING    -48

`define EMPTY    0

module tb_king();
    logic clk, rst_n, slave_waitrequest, slave_read, slave_write, 
        master_waitrequest, master_read, master_readdatavalid, 
        master_write;
    logic [3:0] slave_address;
    logic [31:0] slave_readdata, slave_writedata, master_address,
        master_readdata, master_writedata;

    king dut (clk, rst_n, slave_waitrequest, slave_address, slave_read, 
        slave_readdata, slave_write, slave_writedata, master_waitrequest, 
        master_address, master_read, master_readdata, master_readdatavalid, 
        master_write, master_writedata);

    logic [7:0] write_mem [`MEM_SIZE - 1:0];
    logic [7:0] read_mem [63:0];

    logic [7:0] expected [`NUM_KING_MOVES - 1:0][63:0];

    task reset_mem();
        for (int i = 0; i < `MEM_SIZE; i++) begin
            write_mem[i] = 8'hFF;
        end
    endtask

    task init();
        master_waitrequest = 0;
        master_readdatavalid = 1;
        master_readdata = 32'hFFFFFFFF;
        
        slave_address = 32'd0;
        slave_read = 0;
        slave_write = 0;
        slave_writedata = 32'd0;

        rst_n = 0;
        #20;
        rst_n = 1;
        #20;
    endtask

    task execute_generation();
        // write address of board
        slave_write = 1;
        slave_address = 4'd1;
        slave_writedata = 32'd0;
        #20;
        wait(slave_waitrequest == 0);

        // write destination board address
        slave_write = 1;
        slave_address = 4'd2;
        slave_writedata = 32'd0;
        #20;
        wait(slave_waitrequest == 0);

        // write x coordinate of piece to generate for
        slave_write = 1;
        slave_address = 4'd3;
        slave_writedata = 32'd4;
        #20;
        wait(slave_waitrequest == 0);

        // write y coordinate of piece to generate for
        slave_write = 1;
        slave_address = 4'd4;
        slave_writedata = 32'd1;
        #20;
        wait(slave_waitrequest == 0);

        // write to address 0 to start
        slave_write = 1;
        slave_address = 4'd0;
        #20;
        wait(slave_waitrequest == 0);
        
        // read from address 0 to wait for completion
        slave_write = 0;
        slave_read = 1;
        slave_address = 4'd0;
        #20;
        wait(slave_waitrequest == 0);
    endtask

    // clock 
    initial begin
        clk = 0;
        forever begin
            #5;
            clk = ~clk;
        end
    end

    // sdram mock
    initial begin
        forever begin
            #5;
            if (master_write) begin
                write_mem[master_address / 4] = master_writedata;
            end else if (master_read) begin
                master_readdata = read_mem[master_address / 4];
            end 
        end
    end

    // initialize initial board and expected boards
    initial $readmemh ("C:\\Users\\madel\\cpen391\\ChessMate\\de1\\hardware\\testbenches\\initial-boards\\king-test-board.memh", read_mem);
    initial $readmemh ("C:\\Users\\madel\\cpen391\\ChessMate\\de1\\hardware\\testbenches\\reference-boards\\king-expected1.memh", expected[0]);
    initial $readmemh ("C:\\Users\\madel\\cpen391\\ChessMate\\de1\\hardware\\testbenches\\reference-boards\\king-expected2.memh", expected[1]);
    initial $readmemh ("C:\\Users\\madel\\cpen391\\ChessMate\\de1\\hardware\\testbenches\\reference-boards\\king-expected3.memh", expected[2]);
    initial $readmemh ("C:\\Users\\madel\\cpen391\\ChessMate\\de1\\hardware\\testbenches\\reference-boards\\king-expected4.memh", expected[3]);
    initial $readmemh ("C:\\Users\\madel\\cpen391\\ChessMate\\de1\\hardware\\testbenches\\reference-boards\\king-expected5.memh", expected[4]);
    initial $readmemh ("C:\\Users\\madel\\cpen391\\ChessMate\\de1\\hardware\\testbenches\\reference-boards\\king-expected6.memh", expected[5]);
    initial $readmemh ("C:\\Users\\madel\\cpen391\\ChessMate\\de1\\hardware\\testbenches\\reference-boards\\king-expected7.memh", expected[6]);
    initial $readmemh ("C:\\Users\\madel\\cpen391\\ChessMate\\de1\\hardware\\testbenches\\reference-boards\\king-expected8.memh", expected[7]);

    task board_equals(input int base, input int expected_i, output logic match);
        for (int i = 0; i < 64; i++) begin
            if (write_mem[base + i] !== expected[expected_i][i]) begin
                match = 0;
                return;
            end
        end

        match = 1;
    endtask

    logic match;

    // sequential part
    initial begin
        init();
        execute_generation();

        // check for correct behaviour
        for (int i = 0; i < `NUM_KING_MOVES; i++) begin
            match = 0;

            for (int j = 0; j < `NUM_KING_MOVES; j++) begin
                board_equals(i * 64, j, match);
                if (match) begin
                    $display("Match!");
                    break;
                end
            end

            if (!match) $error("No match for actual board %0d", i);
        end 

    end
endmodule  
