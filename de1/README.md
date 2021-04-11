# How to set up AI/Linux on DE1-SoC

### STEP 0: Put Linux image on MicroSD card
1. Follow pages 1 - 8 in the Linux.pdf file in https://people.ece.cornell.edu/land/courses/ece5760/DE1_SOC/DE1-SoC-UP-Linux/

### STEP 1: Generate the rbf file to program the board
1. Create Quartus project in the de1-system folder.  Add the following files:
    1. ../rook.sv
    2. ../queen.sv
    3. ../king.sv
    4. ../pawn.sv
    5. ../bishop.sv
    6. ../knight.sv
    7. sys.qsys
    8. MyComputer_Verilog.v
    9. HexTo7SegmentDisplay.vhd
2. Add pin assignments (DE1_SoC_Quartus_Settings_File.qsf)
3. Open sys.qsys in the platform designer, and generate the HDL
4. Attempt to compile the Quartus project.  This will likely fail partway through
    1. If it fails, click on Tools->Tcl Scripts and run sys/synthesis/submodules/hps_sdram_p0_pin_assignments.tcl
    2. Compile the project again
6. Follow page 9 of the tutorial to generate an rbf file from output_files/MyComputer_Verilog.sof

### STEP 2: Get it all on the MicroSD
1. Insert microSD card + reader to computer
2. Copy the de1 folder from within the git repo onto FAT_VOLUME (this is the MicroSD card)

### STEP 3: Swap out the default de1 programming
1. Connect using Putty like instructed in section 2.3 of the tutorial
2. Follow section 3.3 of the tutorial to replace the default programming file with the .rbf file we generated in STEP 1
3. Restart the board to implement the changes

### STEP 4: run whatever file to start the state machine
