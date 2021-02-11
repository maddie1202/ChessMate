#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "test/test.h"

board_t* init_board()
{
    board_t* new_game = malloc(sizeof(board_t));

    board_t reference = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            (*new_game)[i][j] = reference[i][j];
        }
    }

    return new_game;
}

board_t* copy_board(board_t* original)
{

}

void init_and_set_board()
{
    current_game = init_board();
}

void destroy_board()
{
    free(current_game);
}

