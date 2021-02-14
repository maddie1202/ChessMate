#ifndef POSSIBLE_MOVE_GENERATORS
#define POSSIBLE_MOVE_GENERATORS

#include "game.h"

typedef struct move_list {
    board_t** moves;
    int num_moves;
} move_list_t;

move_list_t *generate_pawn_moves(board_t *current, int pawn);
move_list_t *generate_rook_moves(board_t *current, int rook);
move_list_t *generate_knight_moves(board_t *current, int knight);
move_list_t *generate_bishop_moves(board_t *current, int bishop);
move_list_t *generate_queen_moves(board_t *current, int pawn);
move_list_t *generate_king_moves(board_t *current, int pawn);

#endif
