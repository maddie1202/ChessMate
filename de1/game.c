#include <stdio.h>
#include <stdlib.h>
#include "include/game.h"
#include "test/include/test.h"
#include <stdbool.h>
#include <math.h>

#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char wpawns[] = {WPAWN0, WPAWN1, WPAWN2, WPAWN3, WPAWN4, WPAWN5, WPAWN6, WPAWN7};
char bpawns[] = {BPAWN0, BPAWN1, BPAWN2, BPAWN3, BPAWN4, BPAWN5, BPAWN6, BPAWN7};
char wrooks[] = {WROOK0, WROOK1, WROOK2, WROOK3, WROOK4, WROOK5, WROOK6, WROOK7, WROOK8, WROOK9};
char brooks[] = {BROOK0, BROOK1, BROOK2, BROOK3, BROOK4, BROOK5, BROOK6, BROOK7, BROOK8, BROOK9};
char wknights[] = {WKNIGHT0, WKNIGHT1, WKNIGHT2, WKNIGHT3, WKNIGHT4, WKNIGHT5, WKNIGHT6, WKNIGHT7, WKNIGHT8, WKNIGHT9};
char bknights[] = {BKNIGHT0, BKNIGHT1, BKNIGHT2, BKNIGHT3, BKNIGHT4, BKNIGHT5, BKNIGHT6, BKNIGHT7, BKNIGHT8, BKNIGHT9};
char wbishops[] = {WBISHOP0, WBISHOP1, WBISHOP2, WBISHOP3, WBISHOP4, WBISHOP5, WBISHOP6, WBISHOP7, WBISHOP8, WBISHOP9};
char bbishops[] = {BBISHOP0, BBISHOP1, BBISHOP2, BBISHOP3, BBISHOP4, BBISHOP5, BBISHOP6, BBISHOP7, BBISHOP8, BBISHOP9};
char wqueens[] = {WQUEEN0, WQUEEN1, WQUEEN2, WQUEEN3, WQUEEN4, WQUEEN5, WQUEEN6, WQUEEN7, WQUEEN8};
char bqueens[] = {BQUEEN0, BQUEEN1, BQUEEN2, BQUEEN3, BQUEEN4, BQUEEN5, BQUEEN6, BQUEEN7, BQUEEN8};

board_t* init_board()
{
    board_t* new_game = malloc(sizeof(board_t));

    board_t reference = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
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

game_t *init_game()
{
    game_t *game = malloc(sizeof(game_t));

    game->board = init_board();
    game->wking_has_moved = false;
    game->wrook0_has_moved = false;
    game->wrook1_has_moved = false;
    game->bking_has_moved = false;
    game->brook0_has_moved = false;
    game->brook1_has_moved = false;

    return game;
}

void init_and_set_game()
{
    current_game = init_game();
}

void destroy_board()
{
    free(current_game);
}

void destroy_game(game_t *game)
{
    free(game->board);
    free(game);
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
    printf("\n");

    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j < 8; j++) {
            char display[3];
            char piece = (*board)[i][j];

            display[0] = get_colour(piece) == WHITE ? 'w' : 'b';
            if (is_pawn(piece)) {
                display[1] = 'P';
                display[2] = abs(piece) - WPAWN0 + '0';
            } else if (is_knight(piece)) {
                display[1] = 'N';
                display[2] = abs(piece) - WKNIGHT0 + '0';
            } else if (is_bishop(piece)) {
                display[1] = 'B';
                display[2] = abs(piece) - WBISHOP0 + '0';
            } else if (is_rook(piece)) {
                display[1] = 'R';
                display[2] = abs(piece) - WROOK0 + '0';
            } else if (is_queen(piece)) {
                display[1] = 'Q';
                display[2] = abs(piece) - WQUEEN0 + '0';
            } else if (is_king(piece)) {
                display[1] = 'K';
                display[2] = ' ';
            } else {
                display[0] = ' ';
                display[1] = ' ';
                display[2] = ' ';
            }

            if (display[0] == 'b') {
                printf(ANSI_COLOR_CYAN "%s " ANSI_COLOR_RESET, display);
            } else {
                printf("%s ", display);
            }
        }

        printf("\n");
    }

    printf("\n\n");
}

bool is_pawn(char piece)
{
    for (int i = 0; i < NUM_PAWNS; i++) {
        if (piece == wpawns[i] || piece == bpawns[i]) return true;
    }

    return false;
}

bool is_knight(char piece)
{
    for (int i = 0; i < NUM_KNIGHTS; i++) {
        if (piece == wknights[i] || piece == bknights[i]) return true;
    }

    return false;
}

bool is_king(char piece)
{
    return piece == WKING || piece == BKING;
}

bool is_bishop(char piece)
{
    for (int i = 0; i < NUM_BISHOPS; i++) {
        if (piece == wbishops[i] || piece == bbishops[i]) return true;
    }

    return false;
}

bool is_rook(char piece)
{
    for (int i = 0; i < NUM_ROOKS; i++) {
        if (piece == wrooks[i] || piece == brooks[i]) return true;
    }

    return false;
}

bool is_queen(char piece)
{
    for (int i = 0; i < NUM_QUEENS; i++) {
        if (piece == wqueens[i] || piece == bqueens[i]) return true;
    }

    return false;
}

char get_colour(char piece)
{
    return piece == 0 ? EMPTY : (piece > 0 ? WHITE : BLACK);
}

int reverse_colour(int colour)
{
    return colour == WHITE ? BLACK : WHITE;
}

// requires king is a valid king
bool has_king_moved(game_t *game, char king)
{
    if (king == WKING) {
        return game->wking_has_moved;
    } else if (king == BKING) {
        return game->bking_has_moved;
    } else {
        return false;
    }
}

// requires rook is a valid rook
bool has_rook_moved(game_t *game, char rook)
{
    if (rook == WROOK0) {
        return game->wrook0_has_moved;
    } else if (rook == WROOK1) {
        return game->wrook1_has_moved;
    } else if (rook == BROOK0) {
        return game->brook0_has_moved;
    } else if (rook == BROOK1) {
        return game->brook1_has_moved;
    } else {
        return false;
    }
}


