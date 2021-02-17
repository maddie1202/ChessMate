#include "game.h"

bool in_check(board_t *board, int colour);
bool in_checkmate(board_t *board, int colour);
board_t *generate_ai_move(game_t *game, int colour, int depth);

double eval_board(board_t *board, int colour);  