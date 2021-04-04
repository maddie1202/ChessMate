#include "include/game.h"
#include <stdlib.h>
#include "include/possible-move-generators.h"

/*
 * Upgrades the pawn on the board to a rook, knight, bishop and queen.
 * If a specific upgrade wasn't selected, defaults to queen.
 * Alters move_list directly.
 */
static move_list_t* upgrade_pawns(board_t *board, move_list_t *move_list, char pawn)
{
    move_list_t *upgraded_move_list = create_move_list(12); // max 12 moves for a pawn

    char colour = get_colour(pawn);

    for (int i = 0; i < move_list->num_moves; i++) {
        int dest_x = -1, dest_y = -1;
        if (!find_piece(move_list->moves[i], pawn, &dest_x, &dest_y)) return NULL;

        if ((colour == WHITE && (dest_y == 7)) || (colour == BLACK && (dest_y == 0))) {
            char rook = colour == WHITE ? pawn + 10 : pawn - 10;
            add_move_to_list(upgraded_move_list, move_list->moves[i], pawn, dest_x, dest_y);
            (*upgraded_move_list->moves[upgraded_move_list->num_moves - 1])[dest_y][dest_x] = rook;

            char knight = colour == WHITE ? pawn + 20 : pawn - 20;
            add_move_to_list(upgraded_move_list, move_list->moves[i], pawn, dest_x, dest_y);
            (*upgraded_move_list->moves[upgraded_move_list->num_moves - 1])[dest_y][dest_x] = knight;

            char bishop = colour == WHITE ? pawn + 30 : pawn -30;
            add_move_to_list(upgraded_move_list, move_list->moves[i], pawn, dest_x, dest_y);
            (*upgraded_move_list->moves[upgraded_move_list->num_moves - 1])[dest_y][dest_x] = bishop;

            char queen = colour == WHITE ? pawn + 39 : pawn - 39;
            add_move_to_list(upgraded_move_list, move_list->moves[i], pawn, dest_x, dest_y);
            (*upgraded_move_list->moves[upgraded_move_list->num_moves - 1])[dest_y][dest_x] = queen;
        } else {
            add_move_to_list(upgraded_move_list, move_list->moves[i], pawn, dest_x, dest_y);
        }
    }

    destroy_move_list(move_list);
    return upgraded_move_list;
}

/*
 * Generates all legal moves for the given pawn.
 * If the pawn reaches the other side of the board, it is upgraded
 * to a rook, bishop, knight, or queen.
 */
move_list_t *generate_pawn_moves(board_t *board, char pawn)
{
    if (board == NULL || !is_pawn(pawn)) return NULL;

    char colour = get_colour(pawn);
    int src_x = -1, src_y = -1;
    if (!find_piece(board, pawn, &src_x, &src_y)) return NULL;

    int home_row = colour == WHITE ? 1 : 6;

    move_list_t *move_list = create_move_list(12); // max 12 moves for a pawn

    // move forward by 1
    int forward = colour == WHITE ? 1 : -1;
    if (get_piece(board, src_x, src_y + forward) == EMPTY) {
        // pawn upgrade
        // if ((colour == WHITE && (src_y + forward == 7)) || (colour == BLACK && (src_y + forward == 0))) {
        //     upgrade_pawn(board, move_list, src_x, src_y + forward, pawn);
        // } else {
            add_move_to_list(move_list, board, pawn, src_x, src_y + forward);
        // }
    }

    // diagonal capture x + 1
    char dest_piece = get_piece(board, src_x + 1, src_y + forward);
    if (dest_piece != OUT_OF_BOUNDS && dest_piece != EMPTY && get_colour(dest_piece) == reverse_colour(colour)) {
        // pawn upgrade
        // if ((colour == WHITE && (src_y + forward == 7)) || (colour == BLACK && (src_y + forward == 0))) {
        //     upgrade_pawn(board, move_list, src_x + 1, src_y + forward, pawn);
        // } else {
            add_move_to_list(move_list, board, pawn, src_x + 1, src_y + forward);
        // }
    }

    // diagonal capture x - 1
    dest_piece = get_piece(board, src_x - 1, src_y + forward);
    if (dest_piece != OUT_OF_BOUNDS && dest_piece != EMPTY && get_colour(dest_piece) == reverse_colour(colour)) {
        // pawn upgrade
        // if ((colour == WHITE && (src_y + forward == 7)) || (colour == BLACK && (src_y + forward == 0))) {
        //     upgrade_pawn(board, move_list, src_x - 1, src_y + forward, pawn);
        // } else {
            add_move_to_list(move_list, board, pawn, src_x - 1, src_y + forward);
        // }
    }

    // move forward by 2 (if pawn is in its initial position only)
    int two_forward = 2 * forward;
    if (src_y == home_row && get_piece(board, src_x, src_y + two_forward) == EMPTY && 
        get_piece(board, src_x, src_y + forward) == EMPTY) {
        add_move_to_list(move_list, board, pawn, src_x, src_y + two_forward); 
    }

    return upgrade_pawns(board, move_list, pawn);
}