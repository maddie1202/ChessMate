#include "include/game.h"
#include <stdlib.h>
#include <stdio.h>

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

board_t *generate_ai_move(game_t *game, int colour, int depth)
{
    return NULL;
}

bool in_check(board_t *board, int colour)
{
    return false;
}

bool in_checkmate(board_t *board, int colour)
{
    return false;
}