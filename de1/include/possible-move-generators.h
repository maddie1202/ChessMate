#ifndef POSSIBLE_MOVE_GENERATORS
#define POSSIBLE_MOVE_GENERATORS

#include "game.h"

typedef struct move_list {
    board_t** moves;
    int num_moves;
} move_list_t;

move_list_t *create_move_list(int size);
void destroy_move_list(move_list_t *move_list);

void add_move_to_list(move_list_t* move_list, board_t *board, char piece, int x, int y);
int check_move(board_t *board, int x, int y, char piece, move_list_t *moves);

move_list_t *generate_pawn_moves(board_t *board, char pawn);
move_list_t *generate_rook_moves(board_t *board, char rook);
move_list_t *generate_knight_moves(board_t *board, char knight);
move_list_t *generate_bishop_moves(board_t *board, char bishop);
move_list_t *generate_queen_moves(board_t *board, char queen);
move_list_t *generate_king_moves(board_t *board, char king);
move_list_t *generate_castling_moves(game_t *game, char king, char rook);

move_list_t *generate_all_moves(game_t *game, int colour);
move_list_t *filter_move_list(move_list_t *move_list, int colour);
move_list_t *generate_all_moves_but_castling(board_t *board, int colour);

#endif
