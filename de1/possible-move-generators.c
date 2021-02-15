#include "include/possible-move-generators.h"
#include <stdlib.h>
#include <stdio.h>
#include "include/game.h"

#define WHITE 0
#define BLACK 1

/* Implement extra checks later for pawn upgrades!!! */
static char get_colour(char piece)
{
    return piece < 16 ? WHITE : (piece == 32 ? EMPTY : BLACK);
}

static void add_move_to_list(move_list_t* move_list, board_t *current, char piece, int x, int y)
{
    move_list->moves[move_list->num_moves] = copy_board(current);
    move_piece(move_list->moves[move_list->num_moves], piece, x, y);
    move_list->num_moves++;
}

// returns 0 if empty, 1 if there was a piece at (x, y)
static int check_move(board_t *current, int x, int y, char piece, move_list_t *moves)
{
    // get colour of piece to be moved
    char colour = get_colour(piece);

    // get curr piece and colour
    char curr_piece = get_piece(current, x, y);
    char curr_colour = get_colour(curr_piece);
    
    // if curr_piece is the same colour as piece, can't move here, or any further
    if (curr_colour == colour) return 1;
    else {
        add_move_to_list(moves, current, piece, x, y);
        if (curr_piece != EMPTY) return 1;
        return 0;
    }
}

move_list_t *generate_pawn_moves(board_t *current, char pawn)
{
    char colour = get_colour(pawn);
    int src_x, src_y;
    find_piece(current, pawn, &src_x, &src_y);

    int home_row = colour == WHITE ? 1 : 6;

    move_list_t *move_list = malloc(sizeof(move_list_t));
    move_list->moves = malloc(4*sizeof(board_t*)); // max 4 moves for a pawn
    move_list->num_moves = 0;

    // move forward by 1
    int forward = colour == WHITE ? 1 : -1;
    if (get_piece(current, src_x, src_y + forward) == EMPTY) {
        add_move_to_list(move_list, current, pawn, src_x, src_y + forward);
    }

    // diagonal capture x + 1
    char dest_piece = get_piece(current, src_x + 1, src_y + forward);
    if (dest_piece != OUT_OF_BOUNDS && dest_piece != EMPTY && get_colour(dest_piece) == !colour) {
        add_move_to_list(move_list, current, pawn, src_x + 1, src_y + forward);
    }

    // diagonal capture x - 1
    dest_piece = get_piece(current, src_x - 1, src_y + forward);
    if (dest_piece != OUT_OF_BOUNDS && dest_piece != EMPTY && get_colour(dest_piece) == !colour) {
        add_move_to_list(move_list, current, pawn, src_x - 1, src_y + forward);
    }

    // move forward by 2
    int two_forward = 2 * forward;
    if (src_y == home_row && get_piece(current, src_x, src_y + two_forward) == EMPTY) {
        add_move_to_list(move_list, current, pawn, src_x, src_y + two_forward); 
    }

    return move_list;
}

move_list_t *generate_rook_moves(board_t *current, char rook)
{
    // check that board isn't NULL
    if (current == NULL) return NULL;
    
    // check that we were given a rook (check pawn upgrades later)
    if (rook != WROOK0 && rook != WROOK1 && rook != BROOK0 && rook != BROOK1) return NULL;
    
    // find current position on board and colour of piece
    int rook_x, rook_y;
    if (!find_piece(current, rook, &rook_x, &rook_y)) return NULL;
    
    move_list_t *move_list = malloc(sizeof(move_list_t));
    move_list->moves = malloc(14*sizeof(board_t*));
    move_list->num_moves = 0;
    
    // check for backwards moves
    for (int yb = rook_y - 1; yb >= 0; yb--) {
        if (check_move(current, rook_x, yb, rook, move_list)) break;
    }
    
    // check for forwards moves
    for (int yf = rook_y + 1; yf < 8; yf++) {
        if (check_move(current, rook_x, yf, rook, move_list)) break;
    }

    // check for left moves
    for (int xl = rook_x - 1; xl >= 0; xl--) {
        if (check_move(current, xl, rook_y, rook, move_list)) break;
    }

    // check for right moves
    for (int xr = rook_x + 1; xr < 8; xr++) {
        if (check_move(current, xr, rook_y, rook, move_list)) break;
    }

    return move_list;
}

move_list_t *generate_knight_moves(board_t *current, char knight)
{
    char colour = get_colour(knight);
    int src_x, src_y;
    find_piece(current, knight, &src_x, &src_y);

    move_list_t *move_list = malloc(sizeof(move_list_t));
    move_list->moves = malloc(8*sizeof(board_t*)); // max 8 moves for a knight
    move_list->num_moves = 0;

    int offsets[8][2] = {{1, 2}, {-1, 2}, {1, -2}, {-1, -2}, {2, 1}, 
        {-2, 1}, {2, -1}, {-2, -1}};

    for (int i = 0; i < 8; i++) {
        int dest_x = src_x + offsets[i][0];
        int dest_y = src_y + offsets[i][1];

        char dest_piece = get_piece(current, dest_x, dest_y);

        if (dest_piece != OUT_OF_BOUNDS && 
            (dest_piece == EMPTY || get_colour(dest_piece) != colour)) {
            add_move_to_list(move_list, current, knight, dest_x, dest_y); 
        } 
    }

    return move_list;
}

move_list_t *generate_bishop_moves(board_t *current, char bishop)
{
    // check that board isn't NULL
    if (current == NULL) return NULL;
    
    // check that we were given a rook (check pawn upgrades later)
    if (bishop != WBISHOP0 && bishop != WBISHOP1 && bishop != BBISHOP0 && bishop != BBISHOP1) return NULL;
    
    // find current position on board and colour of piece
    int bishop_x, bishop_y;
    if (!find_piece(current, bishop, &bishop_x, &bishop_y)) return NULL;
    
    move_list_t *move_list = malloc(sizeof(move_list_t));
    move_list->moves = malloc(14*sizeof(board_t*));
    move_list->num_moves = 0;

    // check for diagonal moves (x and y increasing)
    for (int i = 1; i < 8; i++) {
        if (bishop_x + i >= 8 || bishop_y + i >= 8) break;
        if (check_move(current, bishop_x + i, bishop_y + i, bishop, move_list)) break;
    }

    // check for diagonal moves (x increasing, y decreasing)
    for (int i = 1; i < 8; i++) {
        if (bishop_x + i >= 8 || bishop_y - i < 0) break;
        if (check_move(current, bishop_x + i, bishop_y - i, bishop, move_list)) break;
    }

    // check for diagonal moves (x decreasing, y increasing)
    for (int i = 1; i < 8; i++) {
        if (bishop_x - i < 0 || bishop_y + i >= 8) break;
        if (check_move(current, bishop_x - i, bishop_y + i, bishop, move_list)) break;
    }

    // check for diagonal moves (x increasing, y decreasing)
    for (int i = 1; i < 8; i++) {
        if (bishop_x - i < 0 || bishop_y - i < 0) break;
        if (check_move(current, bishop_x - i, bishop_y - i, bishop, move_list)) break;
    }

    return move_list;
}

move_list_t *generate_queen_moves(board_t *current, char queen)
{
    return NULL;
}

move_list_t *generate_king_moves(board_t *current, char king)
{
    char colour = get_colour(king);
    int src_x, src_y;
    find_piece(current, king, &src_x, &src_y);

    move_list_t *move_list = malloc(sizeof(move_list_t));
    move_list->moves = malloc(8*sizeof(board_t*)); // max 8 moves for a king
    move_list->num_moves = 0;

    int offsets[8][2] = {{1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}};

    for (int i = 0; i < 8; i++) {
        int dest_x = src_x + offsets[i][0];
        int dest_y = src_y + offsets[i][1];

        char dest_piece = get_piece(current, dest_x, dest_y);

        if (dest_piece != OUT_OF_BOUNDS && 
            (dest_piece == EMPTY || get_colour(dest_piece) != colour)) {
            add_move_to_list(move_list, current, king, dest_x, dest_y); 
        } 
    }

    return move_list;
}