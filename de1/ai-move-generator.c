#include "include/game.h"
#include "include/possible-move-generators.h"
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

double pawn_eval[8][8] = {
        { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0 },
        { 5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0,  5.0 },
        { 1.0,  1.0,  2.0,  3.0,  3.0,  2.0,  1.0,  1.0 },
        { 0.5,  0.5,  1.0,  2.5,  2.5,  1.0,  0.5,  0.5 },
        { 0.0,  0.0,  0.0,  2.0,  2.0,  0.0,  0.0,  0.0 },
        { 0.5, -0.5, -1.0,  0.0,  0.0, -1.0, -0.5,  0.5 },
        { 0.5,  1.0, 1.0,  -2.0, -2.0,  1.0,  1.0,  0.5 },
        { 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0 }
    };

double knight_eval[8][8] = {
        { -5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0 },
        { -4.0, -2.0,  0.0,  0.0,  0.0,  0.0, -2.0, -4.0 },
        { -3.0,  0.0,  1.0,  1.5,  1.5,  1.0,  0.0, -3.0 },
        { -3.0,  0.5,  1.5,  2.0,  2.0,  1.5,  0.5, -3.0 },
        { -3.0,  0.0,  1.5,  2.0,  2.0,  1.5,  0.0, -3.0 },
        { -3.0,  0.5,  1.0,  1.5,  1.5,  1.0,  0.5, -3.0 },
        { -4.0, -2.0,  0.0,  0.5,  0.5,  0.0, -2.0, -4.0 },
        { -5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0 }
    };

double bishop_eval[8][8] = {
        { -2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0 },
        { -1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0 },
        { -1.0,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -1.0 },
        { -1.0,  0.5,  0.5,  1.0,  1.0,  0.5,  0.5, -1.0 },
        { -1.0,  0.0,  1.0,  1.0,  1.0,  1.0,  0.0, -1.0 },
        { -1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0 },
        { -1.0,  0.5,  0.0,  0.0,  0.0,  0.0,  0.5, -1.0 },
        { -2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0 }
    };

double rook_eval[8][8] = {
        {  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0 },
        {  0.5,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.5 },
        { -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5 },
        { -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5 },
        { -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5 },
        { -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5 },
        { -0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5 },
        {  0.0,   0.0, 0.0,  0.5,  0.5,  0.0,  0.0,  0.0 }
    };

double queen_eval[8][8] = {
        { -2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0 },
        { -1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0 },
        { -1.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0 },
        { -0.5,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5 },
        {  0.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5 },
        { -1.0,  0.5,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0 },
        { -1.0,  0.0,  0.5,  0.0,  0.0,  0.0,  0.0, -1.0 },
        { -2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0 }
    };

double king_eval[8][8] = {
        { -3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0 },
        { -3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0 },
        { -3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0 },
        { -3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0 },
        { -2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0 },
        { -1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0 },
        {  2.0,  2.0,  0.0,  0.0,  0.0,  0.0,  2.0,  2.0 },
        {  2.0,  3.0,  1.0,  0.0,  0.0,  1.0,  3.0,  2.0 }
    };

static double eval_position(int colour, double scores[8][8], int x, int y)
{
    if (colour == WHITE) {
        y = 7 - y;
    }

    return scores[y][x];
}

static double eval_piece(char piece, int colour, int x, int y)
{
    if (is_pawn(piece)) {
        return 10 + eval_position(colour, pawn_eval, x, y);
    } else if (is_knight(piece)) {
        return 30 + eval_position(colour, knight_eval, x, y);
    } else if (is_bishop(piece)) {
        return 30 + eval_position(colour, bishop_eval, x, y);
    } else if (is_rook(piece)) {
        return 50 + eval_position(colour, rook_eval, x, y);
    } else if (is_queen(piece)) {
        return 90 + eval_position(colour, queen_eval, x, y);
    } else if (is_king(piece)) {
        return 900 + eval_position(colour, king_eval, x, y);
    } else { //EMPTY
        return 0;
    }
}

double eval_board(board_t *board, int colour)
{
    if (board == NULL) return -1;

    double score = 0;

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            char piece = get_piece(board, x, y);
            int piece_colour = get_colour(piece);
            double piece_score = eval_piece(piece, piece_colour, x, y);
            piece_score *= piece_colour == colour ? 1.0 : -1.0;
            score += piece_score;
        }
    }

    return score;
}

// Modifies current_game
// Assumes the board in current_game hasn't yet been updated with the latest move
// Can move the new board into the game here as part of updating state
void update_game_state(game_t *current_game, board_t *new_board, char moved_last)
{
    // max of 2 pieces can be moved by a player in one turn
    char pieces_moved[2] = {0, 0};

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // detect difference between boards: we will be recording the moved pieces on new_board so there is no doubling up
            // also, only consider pieces from the player that made the last move
            if ((*current_game->board)[i][j] != (*new_board)[i][j] && get_colour((*new_board)[i][j]) == moved_last) {
                // haven't detected any other changed pieces yet
                if (pieces_moved[0] == 0) {
                    pieces_moved[0] = (*new_board)[i][j];
                }
                // have now found the max number of possible pieces moved by the user
                else {
                    pieces_moved[1] = (*new_board)[i][j];
                    break;
                }

            }
        }
    }

    if (pieces_moved[0] == WROOK0) current_game->wrook0_has_moved = true;
    else if (pieces_moved[0] == WROOK1) current_game->wrook1_has_moved = true;
    else if (pieces_moved[0] == WKING) current_game->wking_has_moved = true;
    else if (pieces_moved[0] == BROOK0) current_game->brook0_has_moved = true;
    else if (pieces_moved[0] == BROOK1) current_game->brook1_has_moved = true;
    else if (pieces_moved[0] == BKING) current_game->bking_has_moved = true;

    if (pieces_moved[1] == WROOK0) current_game->wrook0_has_moved = true;
    else if (pieces_moved[1] == WROOK1) current_game->wrook1_has_moved = true;
    else if (pieces_moved[1] == WKING) current_game->wking_has_moved = true;
    else if (pieces_moved[1] == BROOK0) current_game->brook0_has_moved = true;
    else if (pieces_moved[1] == BROOK1) current_game->brook1_has_moved = true;
    else if (pieces_moved[1] == BKING) current_game->bking_has_moved = true;
}

// recursive helper
static double generate_ai_move_helper(game_t *game, int original_colour, int colour, int depth, double alpha, double beta)
{
    if (depth == 0) return eval_board(game->board, colour);

    move_list_t *possible_moves = generate_all_moves(game, colour);

    bool maximize = colour == original_colour;

    double min_or_max_score = maximize ? -DBL_MAX : DBL_MAX;

    for (int i = 0; i < possible_moves->num_moves; i++) {
        game_t *tmp_game = copy_game_replace_board(game, possible_moves->moves[i]);

        double score = generate_ai_move_helper(tmp_game, original_colour, reverse_colour(colour), depth - 1, alpha, beta);

        if (maximize && score > min_or_max_score) {
            min_or_max_score = score;
        } else if (!maximize && score < min_or_max_score){ 
            min_or_max_score = score;
        }
        
        alpha = fmax(alpha, min_or_max_score);
        beta = fmin(beta, min_or_max_score);

        destroy_game(tmp_game);

        if (beta <= alpha) {
            destroy_move_list(possible_moves);
            return min_or_max_score;
        }
    }

    destroy_move_list(possible_moves);

    return min_or_max_score;
}

// Modifies game
board_t *generate_ai_move(game_t *game, int colour, int depth)
{
    if (game == NULL || game->board == NULL || 
        (colour != BLACK && colour != WHITE) || depth < 1) return NULL;

    move_list_t *possible_moves = generate_all_moves(game, colour);

    if (possible_moves->num_moves == 0) return NULL;

    double max_score = -DBL_MAX;
    int max_score_idx = 0;

    for (int i = 0; i < possible_moves->num_moves; i++) {
        game_t *tmp_game = copy_game_replace_board(game, possible_moves->moves[i]);

        double score = generate_ai_move_helper(tmp_game, colour, reverse_colour(colour), depth, -DBL_MAX, DBL_MAX);

        if (score > max_score) {
            max_score = score;
            max_score_idx = i;
        }

        destroy_game(tmp_game);
    }

    board_t *chosen_move = copy_board(possible_moves->moves[max_score_idx]);
    destroy_move_list(possible_moves);

    return chosen_move;
}

bool in_check(board_t *board, int colour)
{
    if (colour != BLACK && colour != WHITE) return false;

    char king = colour == WHITE ? WKING : BKING;
    int other_colour = reverse_colour(colour);

    // get all moves opponent can make given the current board
    move_list_t *opponent_moves = generate_all_moves_but_castling(board, other_colour);
    
    for (int i = 0; i < opponent_moves->num_moves; i++) {
        // if the king is no longer on the board, we are in check
        int x, y;
        if (!find_piece(opponent_moves->moves[i], king, &x, &y)) {
            destroy_move_list(opponent_moves);
            return true;
        }
    }

    destroy_move_list(opponent_moves);

    return false;
}

bool in_checkmate(board_t *board, int colour)
{
    if (colour != BLACK && colour != WHITE) return false;

    char king = colour == WHITE ? WKING : BKING;

    // if the current position, as well as any possible moves the king can make are in check, we are in checkmate

    // check current position
    if (!in_check(board, colour)) return false;

    // get all of the king's moves: if these are also in check, then we have checkmate
    move_list_t *king_moves = generate_king_moves(board, king);

    for (int i = 0; i < king_moves->num_moves; i++) {
        if (!in_check(king_moves->moves[i], colour)) {
            destroy_move_list(king_moves);
            return false;
        }
    }

    destroy_move_list(king_moves);

    return true;
}