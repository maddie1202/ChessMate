#include <stdio.h>
#include "../../include/game.h"

#define PAWN_FILENAME "initial-boards/pawn-test-board.memh"
#define ROOK_FILENAME "initial-boards/rook-test-board.memh"
#define BISHOP_FILENAME "initial-boards/bishop-test-board.memh"
#define KNIGHT_FILENAME "initial-boards/knight-test-board.memh"
#define QUEEN_FILENAME "initial-boards/queen-test-board.memh"
#define KING_FILENAME "initial-boards/king-test-board.memh"

void write_board_to_file(char* filename, board_t *board)
{
    FILE *fp = fopen(filename, "w");

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {   
            fprintf(fp, "%x\n", (*board)[i][j]);
        }
    }
}

void write_pawn_board(char *filename)
{
    board_t board = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    write_board_to_file(filename, &board);

    board_t* w_expected1 = copy_board(&board);
    move_piece(w_expected1, WPAWN2, 2, 2);
    write_board_to_file("reference-boards/pawn-expected1.memh", w_expected1);

    // move WPAWN2 from (2, 1) to (2, 3)
    board_t* w_expected2 = copy_board(&board);
    move_piece(w_expected2, WPAWN2, 2, 3);
    write_board_to_file("reference-boards/pawn-expected2.memh", w_expected2);
}

void write_rook_board(char *filename)
{
    board_t board = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    WROOK0,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    write_board_to_file(filename, &board);

    board_t expected1 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    WROOK0,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected2 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    WROOK0,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected3 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    WROOK0,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };
    
    board_t expected4 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   WROOK0, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected5 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    WROOK0,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected6 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, WROOK0,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected7 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WROOK0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected8 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WROOK0,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected9 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  WROOK0,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected10 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    WROOK0,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected11 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    WROOK0},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    write_board_to_file("reference-boards/rook-expected1.memh", &expected1);
    write_board_to_file("reference-boards/rook-expected2.memh", &expected2);
    write_board_to_file("reference-boards/rook-expected3.memh", &expected3);
    write_board_to_file("reference-boards/rook-expected4.memh", &expected4);
    write_board_to_file("reference-boards/rook-expected5.memh", &expected5);
    write_board_to_file("reference-boards/rook-expected6.memh", &expected6);
    write_board_to_file("reference-boards/rook-expected7.memh", &expected7);
    write_board_to_file("reference-boards/rook-expected8.memh", &expected8);
    write_board_to_file("reference-boards/rook-expected9.memh", &expected9);
    write_board_to_file("reference-boards/rook-expected10.memh", &expected10);
    write_board_to_file("reference-boards/rook-expected11.memh", &expected11);
}

void write_bishop_board(char *filename) 
{
    board_t board = {
            {WROOK0, WKNIGHT0, EMPTY,    WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY,  WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    WPAWN3, EMPTY,  WBISHOP0, EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };
    
    write_board_to_file(filename, &board);

    board_t expected1 = {
            {WROOK0, WKNIGHT0, EMPTY, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    WBISHOP0,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    WPAWN3,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected2 = {
            {WROOK0, WKNIGHT0, EMPTY, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WBISHOP0,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    WPAWN3,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected3 = {
            {WROOK0, WKNIGHT0, EMPTY, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WBISHOP0, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    WPAWN3,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected4 = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    WPAWN3,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected5 = {
            {WROOK0, WKNIGHT0, EMPTY, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    WPAWN3,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WBISHOP0,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected6 = {
            {WROOK0, WKNIGHT0, EMPTY, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    WPAWN3,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    WBISHOP0,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected7 = {
            {WROOK0, WKNIGHT0, EMPTY, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    WPAWN3,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   WBISHOP0,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected8 = {
            {WROOK0, WKNIGHT0, EMPTY, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    WPAWN3,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    WBISHOP0,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected9 = {
            {WROOK0, WKNIGHT0, EMPTY, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    WPAWN3,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    WBISHOP0},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    write_board_to_file("reference-boards/bishop-expected1.memh", &expected1);
    write_board_to_file("reference-boards/bishop-expected2.memh", &expected2);
    write_board_to_file("reference-boards/bishop-expected3.memh", &expected3);
    write_board_to_file("reference-boards/bishop-expected4.memh", &expected4);
    write_board_to_file("reference-boards/bishop-expected5.memh", &expected5);
    write_board_to_file("reference-boards/bishop-expected6.memh", &expected6);
    write_board_to_file("reference-boards/bishop-expected7.memh", &expected7);
    write_board_to_file("reference-boards/bishop-expected8.memh", &expected8);
    write_board_to_file("reference-boards/bishop-expected9.memh", &expected9);
}

void write_knight_board(char *filename) 
{
    board_t* curr = init_board();

    write_board_to_file(filename, curr);

    // move WKNIGHT0 from (1, 0) to (0, 2)
    board_t* w_expected1 = copy_board(curr);
    move_piece(w_expected1, WKNIGHT0, 0, 2);

    // move WKNIGHT0 from (1, 0) to (2, 2)
    board_t* w_expected2 = copy_board(curr);
    move_piece(w_expected2, WKNIGHT0, 2, 2);

    write_board_to_file("reference-boards/knight-expected1.memh", w_expected1);
    write_board_to_file("reference-boards/knight-expected2.memh", w_expected2);
}

void write_queen_board(char *filename) 
{
    board_t board = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WPAWN4,  EMPTY,    WQUEEN0,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

        write_board_to_file(filename, &board);

    board_t expected1 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WPAWN4,  EMPTY,    EMPTY,    WQUEEN0},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected2 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WPAWN4,  WQUEEN0,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    
    board_t expected3 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    WQUEEN0,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected4 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    WQUEEN0,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected5 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    WQUEEN0,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected6 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   WQUEEN0,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected7 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    WQUEEN0},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected8 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    WQUEEN0},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };
    
    board_t expected9 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  WQUEEN0,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected10 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  WQUEEN0,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected11 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   WQUEEN0, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected12 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  WQUEEN0,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected13 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WQUEEN0, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected14 = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    write_board_to_file("reference-boards/queen-expected1.memh", &expected1);
    write_board_to_file("reference-boards/queen-expected2.memh", &expected2);
    write_board_to_file("reference-boards/queen-expected3.memh", &expected3);
    write_board_to_file("reference-boards/queen-expected4.memh", &expected4);
    write_board_to_file("reference-boards/queen-expected5.memh", &expected5);
    write_board_to_file("reference-boards/queen-expected6.memh", &expected6);
    write_board_to_file("reference-boards/queen-expected7.memh", &expected7);
    write_board_to_file("reference-boards/queen-expected8.memh", &expected8);
    write_board_to_file("reference-boards/queen-expected9.memh", &expected9);
    write_board_to_file("reference-boards/queen-expected10.memh", &expected10);
    write_board_to_file("reference-boards/queen-expected11.memh", &expected11);
    write_board_to_file("reference-boards/queen-expected12.memh", &expected12);
    write_board_to_file("reference-boards/queen-expected13.memh", &expected13);
    write_board_to_file("reference-boards/queen-expected14.memh", &expected14);
}

void write_king_board(char *filename) 
{
    board_t curr = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,  EMPTY,  EMPTY,    WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY,  WKING,  EMPTY,    EMPTY,    WPAWN7},       
            {EMPTY,  EMPTY,    WQUEEN0,   EMPTY,  EMPTY,  EMPTY,    WPAWN6,   WBISHOP1},
            {EMPTY,  EMPTY,    EMPTY,    WPAWN3, WPAWN4, WPAWN5,   EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    BPAWN3, BPAWN4, BPAWN5,   EMPTY,    EMPTY},
            {EMPTY,  BQUEEN0,   EMPTY,    EMPTY,  EMPTY,  EMPTY,    BPAWN6,   BBISHOP1},
            {BPAWN0, BPAWN1,   BPAWN2,   EMPTY,  BKING,  EMPTY,    EMPTY,    BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, EMPTY,  EMPTY,  EMPTY,    BKNIGHT1, BROOK1}
        };

    write_board_to_file(filename, &curr);

    // move WKING left from (4, 1) to (3, 1)
    board_t* w_expected1 = copy_board(&curr);
    move_piece(w_expected1, WKING, 3, 1);

    // move WKING diagonally from (4, 1) to (3, 2)
    board_t* w_expected2 = copy_board(&curr);
    move_piece(w_expected2, WKING, 3, 2);
    
    // move WKING forward from (4, 1) to (4, 2)
    board_t* w_expected3 = copy_board(&curr);
    move_piece(w_expected3, WKING, 4, 2);

    // move WKING diagonally from (4, 1) to (5, 2)
    board_t* w_expected4 = copy_board(&curr);
    move_piece(w_expected4, WKING, 5, 2);

    // move WKING right from (4, 1) to (5, 1)
    board_t* w_expected5 = copy_board(&curr);
    move_piece(w_expected5, WKING, 5, 1);

    // move WKING diagonally from (4, 1) to (5, 0)
    board_t* w_expected6 = copy_board(&curr);
    move_piece(w_expected6, WKING, 5, 0);

    // move WKING backward from (4, 1) to (4, 0)
    board_t* w_expected7 = copy_board(&curr);
    move_piece(w_expected7, WKING, 4, 0);

    // move WKING diagonally from (4, 1) to (3, 0)
    board_t* w_expected8 = copy_board(&curr);
    move_piece(w_expected8, WKING, 3, 0);

    write_board_to_file("reference-boards/king-expected1.memh", w_expected1);
    write_board_to_file("reference-boards/king-expected2.memh", w_expected2);
    write_board_to_file("reference-boards/king-expected3.memh", w_expected3);
    write_board_to_file("reference-boards/king-expected4.memh", w_expected4);
    write_board_to_file("reference-boards/king-expected5.memh", w_expected5);
    write_board_to_file("reference-boards/king-expected6.memh", w_expected6);
    write_board_to_file("reference-boards/king-expected7.memh", w_expected7);
    write_board_to_file("reference-boards/king-expected8.memh", w_expected8);
}

int main()
{
    write_pawn_board(PAWN_FILENAME);   
    write_rook_board(ROOK_FILENAME);
    write_bishop_board(BISHOP_FILENAME);
    write_knight_board(KNIGHT_FILENAME);
    write_queen_board(QUEEN_FILENAME);
    write_king_board(KING_FILENAME);
}