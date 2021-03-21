#include <stdio.h>
#include "../../include/game.h"

#define PAWN_FILENAME "initial-boards/pawn-test-board.memh"

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

int main()
{
    write_pawn_board(PAWN_FILENAME);   
}