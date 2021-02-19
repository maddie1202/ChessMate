#include "include/possible-move-generators.h"
#include "include/ai-move-generator.h"
#include <stdlib.h>
#include <stdio.h>
#include "include/game.h"

move_list_t *create_move_list(int size)
{
    move_list_t *move_list = malloc(sizeof(move_list_t));
    move_list->moves = malloc(size * sizeof(board_t*)); 
    move_list->num_moves = 0;
    return move_list;
}

void destroy_move_list(move_list_t *move_list)
{
    if (move_list == NULL) return;
    
    if (move_list->moves != NULL) {
        for (int i = 0; i < move_list->num_moves; i++) {
            if (move_list->moves[i] != NULL) free(move_list->moves[i]);
        }
        free(move_list->moves);
    }
    
    free(move_list);
}

static void add_move_to_list(move_list_t* move_list, board_t *board, char piece, int x, int y)
{
    move_list->moves[move_list->num_moves] = copy_board(board);
    move_piece(move_list->moves[move_list->num_moves], piece, x, y);
    move_list->num_moves++;
}

// returns 0 if empty, 1 if there was a piece at (x, y)
static int check_move(board_t *board, int x, int y, char piece, move_list_t *moves)
{
    // get colour of piece to be moved
    char colour = get_colour(piece);

    // get piece and colour at (x, y)
    char curr_piece = get_piece(board, x, y);
    char curr_colour = get_colour(curr_piece);
    
    // if curr_piece is the same colour as piece, can't move here, or any further
    if (curr_colour == colour) return 1;
    else {
        add_move_to_list(moves, board, piece, x, y);
        if (curr_piece != EMPTY) return 1;
        return 0;
    }
}

/*
 * Upgrades the pawn on the board to a rook, knight, bishop or queen.
 * If a specific upgrade wasn't selected, defaults to queen.
 * Alters move directly.
 */
static void upgrade_pawn(board_t *move, int x, int y, char pawn, char upgrade_to)
{
    char colour = get_colour(pawn);

    if (upgrade_to == ROOK) pawn = colour == WHITE ? pawn + 10 : pawn - 10;
    else if (upgrade_to == KNIGHT) pawn = colour == WHITE ? pawn + 20 : pawn - 20;
    else if (upgrade_to == BISHOP) pawn = colour == WHITE ? pawn + 30 : pawn -30;
    else pawn = colour == WHITE ? pawn + 39 : pawn - 39; // default upgrade to queen

    // note coordinate swap
    (*move)[y][x] = pawn;
}

/*
 * Generates all legal moves for the given pawn.
 * If the pawn reaches the other side of the board, it is upgraded
 * to a rook, bishop, knight, or queen.
 */
move_list_t *generate_pawn_moves(board_t *board, char pawn, char upgrade_to)
{
    if (board == NULL || !is_pawn(pawn)) return NULL;

    char colour = get_colour(pawn);
    int src_x = -1, src_y = -1;
    if (!find_piece(board, pawn, &src_x, &src_y)) return NULL;

    int home_row = colour == WHITE ? 1 : 6;

    move_list_t *move_list = create_move_list(4); // max 4 moves for a pawn

    // move forward by 1
    int forward = colour == WHITE ? 1 : -1;
    if (get_piece(board, src_x, src_y + forward) == EMPTY) {
        add_move_to_list(move_list, board, pawn, src_x, src_y + forward);
        // pawn upgrade
        if ((colour == WHITE && (src_y + forward == 7)) || (colour == BLACK && (src_y + forward == 0))) {
            board_t *move = move_list->moves[move_list->num_moves - 1];
            upgrade_pawn(move, src_x, src_y + forward, pawn, upgrade_to);
        }
    }

    // diagonal capture x + 1
    char dest_piece = get_piece(board, src_x + 1, src_y + forward);
    if (dest_piece != OUT_OF_BOUNDS && dest_piece != EMPTY && get_colour(dest_piece) == reverse_colour(colour)) {
        add_move_to_list(move_list, board, pawn, src_x + 1, src_y + forward);
        // pawn upgrade
        if ((colour == WHITE && (src_y + forward == 7)) || (colour == BLACK && (src_y + forward == 0))) {
            board_t *move = move_list->moves[move_list->num_moves - 1];
            upgrade_pawn(move, src_x, src_y + forward, pawn, upgrade_to);
        }
    }

    // diagonal capture x - 1
    dest_piece = get_piece(board, src_x - 1, src_y + forward);
    if (dest_piece != OUT_OF_BOUNDS && dest_piece != EMPTY && get_colour(dest_piece) == reverse_colour(colour)) {
        add_move_to_list(move_list, board, pawn, src_x - 1, src_y + forward);
        // pawn upgrade
        if ((colour == WHITE && (src_y + forward == 7)) || (colour == BLACK && (src_y + forward == 0))) {
            board_t *move = move_list->moves[move_list->num_moves - 1];
            upgrade_pawn(move, src_x, src_y + forward, pawn, upgrade_to);
        }
    }

    // move forward by 2 (if pawn is in its initial position only)
    int two_forward = 2 * forward;
    if (src_y == home_row && get_piece(board, src_x, src_y + two_forward) == EMPTY) {
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

/*
 * Generates a castling move for the given king and rook if:
 * 1. neither king nor rook has moved from its original position
 * 2. there are no pieces between the king and rook
 * 3. the king is not castling out of check
 * 4. the king is not castling into check
 * 5. the king is not castling through check
 */
move_list_t *generate_castling_moves(game_t *game, char king, char rook)
{
    // make sure we have been given valid pieces to castle with
    if (!is_king(king) || !is_rook(rook) || get_colour(king) != get_colour(rook)) return NULL;
    
    move_list_t *move_list = create_move_list(1);

    /* 1. neither king nor rook has moved from its original position */
    if (has_king_moved(game, king) || has_rook_moved(game, rook)) return move_list;
    
    int king_x = -1, king_y = -1, rook_x = -1, rook_y = -1;
    if (!find_piece(game->board, king, &king_x, &king_y)) return move_list;
    if (!find_piece(game->board, rook, &rook_x, &rook_y)) return move_list;
    int colour = get_colour(king);

    int kingside = (king_x < rook_x) ? 1 : -1;

    /* 2. there are no pieces between the king and rook */
    int bound = (kingside == 1) ? 3 : 4;
    
    for (int i = 1; i < bound; i++) {
        if (get_piece(game->board, king_x + kingside * i, king_y) != EMPTY) return move_list;
    }
    
    /* 3. the king is not castling out of check */
    if (in_check(game->board, colour)) return move_list;
    
    /* 4/5: the king is not castling into or through check */
    for (int i = 1; i < 3; i++) { // king only moves 2 spaces over
        board_t *check_spot = copy_board(game->board);
        move_piece(check_spot, king, king_x + kingside * i, king_y);
        bool check = in_check(check_spot, colour);
        free(check_spot);
        if (check) return move_list;
    }

    /* Move is valid: construct board */
    board_t *castle = copy_board(game->board);
    move_piece(castle, rook, kingside == 1 ? rook_x - 2 : rook_x + 3, rook_y);

    // moves the king and adds the move to the list
    add_move_to_list(move_list, castle, king, king_x + 2 * kingside, king_y); 
    
    free(castle); // got copied into the list

    return move_list;
}

// WARNING: FREES src
void add_all(move_list_t *dest, move_list_t *src)
{
    if (dest == NULL || dest->moves == NULL || 
        src == NULL || src->moves == NULL) return;

    int new_num_moves = dest->num_moves + src->num_moves;
    board_t **new_moves = malloc(new_num_moves * sizeof(board_t*));

    int index = 0;

    for (int i = 0; i < dest->num_moves; i++) {
        new_moves[index++] = dest->moves[i];
    }

    for (int i = 0; i < src->num_moves; i++) {
        new_moves[index++] = src->moves[i];
    }

    free(dest->moves);
    free(src->moves);
    free(src);

    dest->moves = new_moves;
    dest->num_moves = new_num_moves;
}

/*
 * Generates all moves for a given player.
 */
move_list_t *generate_all_moves(game_t *game, int colour)
{
    if (game == NULL || game->board == NULL || (colour != WHITE && colour != BLACK)) {
        return NULL;
    }

    char *rooks = colour == WHITE ? wrooks : brooks;
    char king = colour == WHITE ? WKING : BKING;

    move_list_t *all_moves = generate_all_moves_but_castling(game->board, colour);

    // filter out any moves where the player puts themselves into check
    move_list_t *filtered_moves = filter_move_list(all_moves, colour);

    // // kingside castle
    move_list_t *kingside_castle_moves = generate_castling_moves(game, king, rooks[1]);
    add_all(filtered_moves, kingside_castle_moves);

    // // queenside castle
    move_list_t *queenside_castle_moves = generate_castling_moves(game, king, rooks[0]);
    add_all(filtered_moves, queenside_castle_moves);
    
    return filtered_moves;
}

/*
 * Filters out any potential moves that put the player into check.
 * Frees move_list.
 */
move_list_t *filter_move_list(move_list_t *move_list, int colour)
{
    int check_count = 0;
    bool *check = malloc(move_list->num_moves);

    // find and flag all moves that put the player into check
    for (int i = 0; i < move_list->num_moves; i++) {
        if (in_check(move_list->moves[i], colour)) {
            check[i] = true;
            check_count++;
        } else {
            check[i] = false;
        }
    }
    
    // copy over moves that do not put the player in check into a new move list
    move_list_t *filtered_list = create_move_list(move_list->num_moves - check_count);
    for (int i = 0; i < move_list->num_moves; i++) {
        if (!check[i]) {
            filtered_list->moves[filtered_list->num_moves] = move_list->moves[i];
            filtered_list->num_moves++;
        } else {
            free(move_list->moves[i]);
        }
    }

    // free move_list
    free(move_list->moves);
    free(move_list);
    free(check);
    
    return filtered_list;
}

/*
 * Generate all moves EXCEPT for castling for a given player.
 */
move_list_t *generate_all_moves_but_castling(board_t *board, int colour)
{
    if (board == NULL || (colour != WHITE && colour != BLACK)) {
        return NULL;
    }

    char *pawns = colour == WHITE ? wpawns : bpawns;
    char *rooks = colour == WHITE ? wrooks : brooks;
    char *knights = colour == WHITE ? wknights : bknights;
    char *bishops = colour == WHITE ? wbishops : bbishops;
    char *queens = colour == WHITE ? wqueens : bqueens;
    char king = colour == WHITE ? WKING : BKING;

    move_list_t *master_list = create_move_list(0);

    // pawns
    for (int i = 0; i < NUM_PAWNS; i++) {
        move_list_t *pawn_moves = generate_pawn_moves(board, pawns[i], EMPTY);
        add_all(master_list, pawn_moves);
    }

    // rooks
    for (int i = 0; i < NUM_ROOKS; i++) {
        move_list_t *rook_moves = generate_rook_moves(board, rooks[i]);
        add_all(master_list, rook_moves);
    }

    // knights
    for (int i = 0; i < NUM_KNIGHTS; i++) {
        move_list_t *knight_moves = generate_knight_moves(board, knights[i]);
        add_all(master_list, knight_moves);
    }

    // bishops
    for (int i = 0; i < NUM_BISHOPS; i++) {
        move_list_t *bishop_moves = generate_bishop_moves(board, bishops[i]);
        add_all(master_list, bishop_moves);
    }

    // queens
    for (int i = 0; i < NUM_QUEENS; i++) {
        move_list_t *queen_moves = generate_queen_moves(board, queens[i]);
        add_all(master_list, queen_moves);
    }

    // king
    move_list_t *king_moves = generate_king_moves(board, king);
    add_all(master_list, king_moves);
    
    return master_list;
}