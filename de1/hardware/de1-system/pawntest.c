#include <stdio.h>
#include "../../../include/game.h"

#define pawn_base (volatile int *) 0xFF202040
#define rook_base (volatile int *) 0xFF202080
#define bishop_base (volatile int *) 0xFF202080 // CHANGE
#define knight_base (volatile int *) 0xFF202080 // CHANGE
#define queen_base (volatile int *) 0xFF202080 // CHANGE
#define king_base (volatile int *) 0xFF202080 // CHANGE

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
    
    board_t bishop_board = {
            {WROOK0, WKNIGHT0, EMPTY,    WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY,  WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    WPAWN3, EMPTY,  WBISHOP0, EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t knight_board = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t queen_board = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WPAWN4,  EMPTY,    WQUEEN0,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t king_board = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,  EMPTY,  EMPTY,    WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY,  WKING,  EMPTY,    EMPTY,    WPAWN7},       
            {EMPTY,  EMPTY,    WQUEEN0,   EMPTY,  EMPTY,  EMPTY,    WPAWN6,   WBISHOP1},
            {EMPTY,  EMPTY,    EMPTY,    WPAWN3, WPAWN4, WPAWN5,   EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    BPAWN3, BPAWN4, BPAWN5,   EMPTY,    EMPTY},
            {EMPTY,  BQUEEN0,   EMPTY,    EMPTY,  EMPTY,  EMPTY,    BPAWN6,   BBISHOP1},
            {BPAWN0, BPAWN1,   BPAWN2,   EMPTY,  BKING,  EMPTY,    EMPTY,    BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, EMPTY,  EMPTY,  EMPTY,    BKNIGHT1, BROOK1}
        };

    // put board in mem
    int i;
    for (i = 0; i < 64; i++) {
        // *(sdram_base + i) = pawn_board[i / 8][i % 8];
        *(sdram_base + i) = rook_board[i / 8][i % 8];
        // *(sdram_base + i) = bishop_board[i / 8][i % 8];
        // *(sdram_base + i) = knight_board[i / 8][i % 8];
        // *(sdram_base + i) = queen_board[i / 8][i % 8];
        // *(sdram_base + i) = king_board[i / 8][i % 8];
    }
    
    // *(pawn_base + 1) = 0; // src
    // *(pawn_base + 2) = 64*4; // dest
    // *(pawn_base + 3) = 2; // x = 2 for pawn
    // *(pawn_base + 4) = 1; // y = 1 for pawn
    // *pawn_base = 0; // start the module
    // int count = *pawn_base; // expect 2

    *(rook_base + 1) = 0; // src
    *(rook_base + 2) = 64*4; // dest
    *(rook_base + 3) = 3; // x = 3 for rook
    *(rook_base + 4) = 3; // y = 3 for rook
    *rook_base = 0; // start the module
    int count = *rook_base; // expect 11

    // *(bishop_base + 1) = 0; // src
    // *(bishop_base + 2) = 64*4; // dest
    // *(bishop_base + 3) = 3; // x = 5 for bishop
    // *(bishop_base + 4) = 3; // y = 3 for bishop
    // *bishop_base = 0; // start the module
    // int count = *bishop_base; // expect 9

    // *(knight_base + 1) = 0; // src
    // *(knight_base + 2) = 64*4; // dest
    // *(knight_base + 3) = 3; // x = 1 for knight
    // *(knight_base + 4) = 3; // y = 0 for knight
    // *knight_base = 0; // start the module
    // int count = *knight_base; // expect 2

    // *(queen_base + 1) = 0; // src
    // *(queen_base + 2) = 64*4; // dest
    // *(queen_base + 3) = 3; // x = 6 for queen
    // *(queen_base + 4) = 3; // y = 3 for queen
    // *queen_base = 0; // start the module
    // int count = *queen_base; // expect 14

    // *(king_base + 1) = 0; // src
    // *(king_base + 2) = 64*4; // dest
    // *(king_base + 3) = 3; // x = 4 for king
    // *(king_base + 4) = 3; // y = 1 for king
    // *king_base = 0; // start the module
    // int count = *king_base; // expect 8

    printf("board count: %d\n", count);

    return(0);
}
