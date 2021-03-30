#include <stdio.h>
#include "../../../include/game.h"

#define pawn_base (volatile int *) 0xFF202040
#define rook_base (volatile int *) 0xFF202080

#define sdram_base (volatile int *) 0xC0000000

int main()
{
    board_t pawn_board = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t rook_board = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    WROOK0,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    // put board in mem
    int i;
    for (i = 0; i < 64; i++) {
        // *(sdram_base + i) = pawn_board[i / 8][i % 8];
        *(sdram_base + i) = rook_board[i / 8][i % 8];
    }

    for (i = 0; i < 64; i++) {
        *(sdram_base + 64 + i) = 1;
    }
    
    // *(pawn_base + 1) = 0; // src
    // *(pawn_base + 2) = 64; // dest
    // *(pawn_base + 3) = 2; // x = 2 for pawn, x = 3 for rook
    // *(pawn_base + 4) = 1; // y = 1 for pawn, y = 3 for rook
    // *pawn_base = 0; // start the module

    // int count = *pawn_base;

    *(rook_base + 1) = 0; // src
    *(rook_base + 2) = 64; // dest
    *(rook_base + 3) = 3; // x = 2 for pawn, x = 3 for rook
    *(rook_base + 4) = 3; // y = 1 for pawn, y = 3 for rook
    *rook_base = 0; // start the module

    int count = *rook_base;

    printf("board count: %d\n", count);

    // print_board((board_t *)(sdram_base + 64));
    // print_board((board_t*)(sdram_base + 2 * 64));

    return(0);
}
