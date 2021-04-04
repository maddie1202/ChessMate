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

void add_move_to_list(move_list_t* move_list, board_t *board, char piece, int x, int y)
{
    move_list->moves[move_list->num_moves] = copy_board(board);
    move_piece(move_list->moves[move_list->num_moves], piece, x, y);
    move_list->num_moves++;
}

// returns 0 if empty, 1 if there was a piece at (x, y)
int check_move(board_t *board, int x, int y, char piece, move_list_t *moves)
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
        move_list_t *pawn_moves = generate_pawn_moves(board, pawns[i]);
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
