#include <stdio.h>
#include <stdlib.h>
#include "include/game.h"
#include "test/include/test.h"
#include <stdbool.h>

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
    board_t* copied_board = malloc(sizeof(board_t));

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            (*copied_board)[i][j] = (*original)[i][j];
        }
    }

    return copied_board;
}

void init_and_set_board()
{
    board_t *board = init_board();
    
    current_game = malloc(sizeof(game_t));

    current_game->board = board;
    current_game->wking_has_moved = false;
    current_game->wrook0_has_moved = false;
    current_game->wrook1_has_moved = false;
    current_game->bking_has_moved = false;
    current_game->brook0_has_moved = false;
    current_game->brook1_has_moved = false;
}

void destroy_board()
{
    free(current_game);
}

int find_piece(board_t *board, char piece, int *x, int *y) 
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((*board)[i][j] == piece) {
                *x = j;
                *y = i;
                return 1;
            }
        }
    }
    
    return 0;
}

void move_piece(board_t *board, char piece, int dest_x, int dest_y) 
{
    int src_x, src_y;
    find_piece(board, piece, &src_x, &src_y);

    (*board)[src_y][src_x] = EMPTY;
    (*board)[dest_y][dest_x] = piece;
}

char get_piece(board_t *board, int x, int y)
{
    if (x < 0 || x > 7 || y < 0 || y > 7) return OUT_OF_BOUNDS;
    return (*board)[y][x];
}

void print_board(board_t *board)
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%2d ", (*board)[i][j]);
        }

        printf("\n");
    }

    printf("\n");
}

bool is_pawn(char piece)
{
    return (WPAWN0 >= 0 && WPAWN7 <= 7) || (BPAWN0 >= 16 && BPAWN7 <= 23);
}

bool is_knight(char piece)
{
    return piece == WKNIGHT0 || piece == WKNIGHT1 || piece == BKNIGHT0 || piece == BKNIGHT1;
}

bool is_king(char piece)
{
    return piece == WKING || piece == BKING;
}

bool is_bishop(char piece)
{
    return piece == WBISHOP0 || piece == WBISHOP1 || piece == BBISHOP0 || piece == BBISHOP1;
}

bool is_rook(char piece)
{
    return piece == WROOK0 || piece == WROOK1 || piece == BROOK0 || piece == BROOK1;
}

bool is_queen(char piece)
{
    return piece == WQUEEN || piece == BQUEEN;
}


