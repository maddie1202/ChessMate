#include "include/possible-move-generators.h"
#include "include/ai-move-generator.h"
#include <stdlib.h>
#include <stdio.h>
#include "include/game.h"

/*
 * Upgrades the pawn on the board to a rook, knight, bishop and queen.
 * If a specific upgrade wasn't selected, defaults to queen.
 * Alters move_list directly.
 */
static void upgrade_pawn(board_t *board, move_list_t *move_list, int dest_x, int dest_y, char pawn)
{
    char colour = get_colour(pawn);

    char rook = colour == WHITE ? pawn + 10 : pawn - 10;
    add_move_to_list(move_list, board, pawn, dest_x, dest_y);
    (*move_list->moves[move_list->num_moves - 1])[dest_y][dest_x] = rook;

    char knight = colour == WHITE ? pawn + 20 : pawn - 20;
    add_move_to_list(move_list, board, pawn, dest_x, dest_y);
    (*move_list->moves[move_list->num_moves - 1])[dest_y][dest_x] = knight;

    char bishop = colour == WHITE ? pawn + 30 : pawn -30;
    add_move_to_list(move_list, board, pawn, dest_x, dest_y);
    (*move_list->moves[move_list->num_moves - 1])[dest_y][dest_x] = bishop;

    char queen = colour == WHITE ? pawn + 39 : pawn - 39;
    add_move_to_list(move_list, board, pawn, dest_x, dest_y);
    (*move_list->moves[move_list->num_moves - 1])[dest_y][dest_x] = queen;
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
        if ((colour == WHITE && (src_y + forward == 7)) || (colour == BLACK && (src_y + forward == 0))) {
            upgrade_pawn(board, move_list, src_x, src_y + forward, pawn);
        } else {
            add_move_to_list(move_list, board, pawn, src_x, src_y + forward);
        }
    }

    // diagonal capture x + 1
    char dest_piece = get_piece(board, src_x + 1, src_y + forward);
    if (dest_piece != OUT_OF_BOUNDS && dest_piece != EMPTY && get_colour(dest_piece) == reverse_colour(colour)) {
        // pawn upgrade
        if ((colour == WHITE && (src_y + forward == 7)) || (colour == BLACK && (src_y + forward == 0))) {
            upgrade_pawn(board, move_list, src_x + 1, src_y + forward, pawn);
        } else {
            add_move_to_list(move_list, board, pawn, src_x + 1, src_y + forward);
        }
    }

    // diagonal capture x - 1
    dest_piece = get_piece(board, src_x - 1, src_y + forward);
    if (dest_piece != OUT_OF_BOUNDS && dest_piece != EMPTY && get_colour(dest_piece) == reverse_colour(colour)) {
        // pawn upgrade
        if ((colour == WHITE && (src_y + forward == 7)) || (colour == BLACK && (src_y + forward == 0))) {
            upgrade_pawn(board, move_list, src_x - 1, src_y + forward, pawn);
        } else {
            add_move_to_list(move_list, board, pawn, src_x - 1, src_y + forward);
        }
    }

    // move forward by 2 (if pawn is in its initial position only)
    int two_forward = 2 * forward;
    if (src_y == home_row && get_piece(board, src_x, src_y + two_forward) == EMPTY && 
        get_piece(board, src_x, src_y + forward) == EMPTY) {
        add_move_to_list(move_list, board, pawn, src_x, src_y + two_forward); 
    }

    return move_list;
}

/*
 * Generates all legal moves for the given rook.
 */
move_list_t *generate_rook_moves(board_t *board, char rook)
{
    if (board == NULL || !is_rook(rook)) return NULL;
    
    // find board position on board and colour of piece
    int rook_x = -1, rook_y = -1;
    if (!find_piece(board, rook, &rook_x, &rook_y)) return NULL;
    
    move_list_t *move_list = create_move_list(14); // max 14 moves for a rook
    
    // check for backwards moves
    for (int yb = rook_y - 1; yb >= 0; yb--) {
        if (check_move(board, rook_x, yb, rook, move_list)) break;
    }
    
    // check for forwards moves
    for (int yf = rook_y + 1; yf < 8; yf++) {
        if (check_move(board, rook_x, yf, rook, move_list)) break;
    }

    // check for left moves
    for (int xl = rook_x - 1; xl >= 0; xl--) {
        if (check_move(board, xl, rook_y, rook, move_list)) break;
    }

    // check for right moves
    for (int xr = rook_x + 1; xr < 8; xr++) {
        if (check_move(board, xr, rook_y, rook, move_list)) break;
    }

    return move_list;
}

/*
 * Generates all legal moves for the given knight.
 */
move_list_t *generate_knight_moves(board_t *board, char knight)
{
    if (board == NULL || !is_knight(knight)) return NULL;

    char colour = get_colour(knight);
    int src_x = -1, src_y = -1;
    if (!find_piece(board, knight, &src_x, &src_y)) return NULL;

    move_list_t *move_list = create_move_list(8); // max 8 moves for a knight

    int offsets[8][2] = {{1, 2}, {-1, 2}, {1, -2}, {-1, -2}, {2, 1}, 
        {-2, 1}, {2, -1}, {-2, -1}};

    for (int i = 0; i < 8; i++) {
        int dest_x = src_x + offsets[i][0];
        int dest_y = src_y + offsets[i][1];

        char dest_piece = get_piece(board, dest_x, dest_y);

        if (dest_piece != OUT_OF_BOUNDS && 
            (dest_piece == EMPTY || get_colour(dest_piece) == reverse_colour(colour))) {
            add_move_to_list(move_list, board, knight, dest_x, dest_y); 
        } 
    }

    return move_list;
}

/*
 * Generates all legal moves for the given bishop.
 */
move_list_t *generate_bishop_moves(board_t *board, char bishop)
{
    if (board == NULL || !is_bishop(bishop)) return NULL;
    
    // find board position on board and colour of piece
    int bishop_x = -1, bishop_y = -1;
    if (!find_piece(board, bishop, &bishop_x, &bishop_y)) return NULL;
    
    move_list_t *move_list = create_move_list(14); // max 14 moves for a bishop

    // check for diagonal moves (x and y increasing)
    for (int i = 1; i < 8; i++) {
        if (bishop_x + i >= 8 || bishop_y + i >= 8) break;
        if (check_move(board, bishop_x + i, bishop_y + i, bishop, move_list)) break;
    }

    // check for diagonal moves (x increasing, y decreasing)
    for (int i = 1; i < 8; i++) {
        if (bishop_x + i >= 8 || bishop_y - i < 0) break;
        if (check_move(board, bishop_x + i, bishop_y - i, bishop, move_list)) break;
    }

    // check for diagonal moves (x decreasing, y increasing)
    for (int i = 1; i < 8; i++) {
        if (bishop_x - i < 0 || bishop_y + i >= 8) break;
        if (check_move(board, bishop_x - i, bishop_y + i, bishop, move_list)) break;
    }

    // check for diagonal moves (x increasing, y decreasing)
    for (int i = 1; i < 8; i++) {
        if (bishop_x - i < 0 || bishop_y - i < 0) break;
        if (check_move(board, bishop_x - i, bishop_y - i, bishop, move_list)) break;
    }

    return move_list;
}

/*
 * Generates all legal moves for the given queen.
 */
move_list_t *generate_queen_moves(board_t *board, char queen)
{
    if (board == NULL || !is_queen(queen)) return NULL;
    
    // find board position on board and colour of piece
    int queen_x = -1, queen_y = -1;
    if (!find_piece(board, queen, &queen_x, &queen_y)) return NULL;
    
    move_list_t *move_list = create_move_list(28); // max 28 moves for a queen

    // check for backwards moves
    for (int yb = queen_y - 1; yb >= 0; yb--) {
        if (check_move(board, queen_x, yb, queen, move_list)) break;
    }
    
    // check for forwards moves
    for (int yf = queen_y + 1; yf < 8; yf++) {
        if (check_move(board, queen_x, yf, queen, move_list)) break;
    }

    // check for left moves
    for (int xl = queen_x - 1; xl >= 0; xl--) {
        if (check_move(board, xl, queen_y, queen, move_list)) break;
    }

    // check for right moves
    for (int xr = queen_x + 1; xr < 8; xr++) {
        if (check_move(board, xr, queen_y, queen, move_list)) break;
    }

    // check for diagonal moves (x and y increasing)
    for (int i = 1; i < 8; i++) {
        if (queen_x + i >= 8 || queen_y + i >= 8) break;
        if (check_move(board, queen_x + i, queen_y + i, queen, move_list)) break;
    }

    // check for diagonal moves (x increasing, y decreasing)
    for (int i = 1; i < 8; i++) {
        if (queen_x + i >= 8 || queen_y - i < 0) break;
        if (check_move(board, queen_x + i, queen_y - i, queen, move_list)) break;
    }

    // check for diagonal moves (x decreasing, y increasing)
    for (int i = 1; i < 8; i++) {
        if (queen_x - i < 0 || queen_y + i >= 8) break;
        if (check_move(board, queen_x - i, queen_y + i, queen, move_list)) break;
    }

    // check for diagonal moves (x increasing, y decreasing)
    for (int i = 1; i < 8; i++) {
        if (queen_x - i < 0 || queen_y - i < 0) break;
        if (check_move(board, queen_x - i, queen_y - i, queen, move_list)) break;
    }

    return move_list;
}

/*
 * Generates all moves for the given king.
 * Does not check for moving into check: this will be
 * filtered out elsewhere.
 */
move_list_t *generate_king_moves(board_t *board, char king)
{
    if (board == NULL || !is_king(king)) return NULL;

    char colour = get_colour(king);
    int src_x = -1, src_y = -1;
    if (!find_piece(board, king, &src_x, &src_y)) return NULL;

    move_list_t *move_list = create_move_list(8); // max 8 moves for a king

    int offsets[8][2] = {{1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}};

    for (int i = 0; i < 8; i++) {
        int dest_x = src_x + offsets[i][0];
        int dest_y = src_y + offsets[i][1];

        char dest_piece = get_piece(board, dest_x, dest_y);

        if (dest_piece != OUT_OF_BOUNDS && 
            (dest_piece == EMPTY || get_colour(dest_piece) == reverse_colour(colour))) {
            add_move_to_list(move_list, board, king, dest_x, dest_y); 
        } 
    }

    return move_list;
}
