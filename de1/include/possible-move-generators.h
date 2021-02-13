#include "game.h"

typedef struct move_list {
    board_t** moves;
    int num_moves;
} move_list_t;

move_list_t *generate_pawn_moves(board_t *current, int pawn);