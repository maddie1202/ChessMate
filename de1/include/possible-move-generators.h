#ifndef POSSIBLE_MOVE_GENERATORS
#define POSSIBLE_MOVE_GENERATORS

#include "game.h"

#define WHITE 1
#define BLACK 2

typedef struct move_list {
    board_t** moves;
    int num_moves;
} move_list_t;

move_list_t *create_move_list(int size);
void destroy_move_list(move_list_t *move_list);

move_list_t *generate_pawn_moves(board_t *board, char pawn);
move_list_t *generate_rook_moves(board_t *board, char rook);
move_list_t *generate_knight_moves(board_t *board, char knight);
move_list_t *generate_bishop_moves(board_t *board, char bishop);
move_list_t *generate_queen_moves(board_t *board, char pawn);
move_list_t *generate_king_moves(board_t *board, char pawn);
move_list_t *generate_castling_moves(game_t *game, char king, char rook);

move_list_t *generate_all_moves(game_t *game, int colour);

#endif
