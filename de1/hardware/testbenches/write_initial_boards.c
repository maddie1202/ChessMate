#include <stdio.h>
#include "../../include/game.h"

#define PAWN_FILENAME "initial-boards/pawn-test-board.memh"

void write_board_to_file(char* filename, board_t *board)
{
    FILE *fp = fopen(filename, "w");

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            char ch = 0xC0;
            fprintf(fp, "%x\n", ch & (*board)[i][j]);
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
}

int main()
{
    write_pawn_board(PAWN_FILENAME);   
}