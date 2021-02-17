#include "include/possible-move-generators.h"
#include <stdlib.h>
#include <stdio.h>
#include "include/game.h"

/* Implement extra checks later for pawn upgrades!!! */
static char get_colour(char piece)
{
    return piece == 0 ? EMPTY : (piece > 0 ? WHITE : BLACK);
}

static int reverse_colour(int colour)
{
    return colour == WHITE ? BLACK : WHITE;
}

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
    
    for (int i = 0; i < move_list->num_moves; i++) {
        free(move_list->moves[i]);
    }

    free(move_list->moves);
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

    // get curr piece and colour
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

move_list_t *generate_pawn_moves(board_t *board, char pawn)
{
    if (board == NULL || !is_pawn(pawn)) return NULL;

    char colour = get_colour(pawn);
    int src_x, src_y;
    find_piece(board, pawn, &src_x, &src_y);

    int home_row = colour == WHITE ? 1 : 6;

    move_list_t *move_list = create_move_list(4); // max 4 moves for a pawn

    // move forward by 1
    int forward = colour == WHITE ? 1 : -1;
    if (get_piece(board, src_x, src_y + forward) == EMPTY) {
        add_move_to_list(move_list, board, pawn, src_x, src_y + forward);
    }

    // diagonal capture x + 1
    char dest_piece = get_piece(board, src_x + 1, src_y + forward);
    if (dest_piece != OUT_OF_BOUNDS && dest_piece != EMPTY && get_colour(dest_piece) == reverse_colour(colour)) {
        add_move_to_list(move_list, board, pawn, src_x + 1, src_y + forward);
    }

    // diagonal capture x - 1
    dest_piece = get_piece(board, src_x - 1, src_y + forward);
    if (dest_piece != OUT_OF_BOUNDS && dest_piece != EMPTY && get_colour(dest_piece) == reverse_colour(colour)) {
        add_move_to_list(move_list, board, pawn, src_x - 1, src_y + forward);
    }

    // move forward by 2
    int two_forward = 2 * forward;
    if (src_y == home_row && get_piece(board, src_x, src_y + two_forward) == EMPTY) {
        add_move_to_list(move_list, board, pawn, src_x, src_y + two_forward); 
    }

    return move_list;
}

move_list_t *generate_rook_moves(board_t *board, char rook)
{
    if (board == NULL || !is_rook(rook)) return NULL;
    
    // find board position on board and colour of piece
    int rook_x, rook_y;
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

move_list_t *generate_knight_moves(board_t *board, char knight)
{
    if (board == NULL || !is_knight(knight)) return NULL;

    char colour = get_colour(knight);
    int src_x, src_y;
    find_piece(board, knight, &src_x, &src_y);

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

move_list_t *generate_bishop_moves(board_t *board, char bishop)
{
    if (board == NULL || !is_bishop(bishop)) return NULL;
    
    // find board position on board and colour of piece
    int bishop_x, bishop_y;
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

move_list_t *generate_queen_moves(board_t *board, char queen)
{
    if (board == NULL || !is_queen(queen)) return NULL;
    
    // find board position on board and colour of piece
    int queen_x, queen_y;
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

move_list_t *generate_king_moves(board_t *board, char king)
{
    if (board == NULL || !is_king(king)) return NULL;

    char colour = get_colour(king);
    int src_x, src_y;
    find_piece(board, king, &src_x, &src_y);

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

move_list_t *generate_castling_moves(game_t *game, char king, char rook)
{
    return NULL;
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

move_list_t *generate_all_moves(game_t *game, int colour)
{
    if (game == NULL || game->board == NULL || (colour != WHITE && colour != BLACK)) {
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
        move_list_t *pawn_moves = generate_pawn_moves(game->board, pawns[i]);
        add_all(master_list, pawn_moves);
    }

    // rooks
    for (int i = 0; i < NUM_ROOKS; i++) {
        move_list_t *rook_moves = generate_rook_moves(game->board, rooks[i]);
        add_all(master_list, rook_moves);
    }

    // knights
    for (int i = 0; i < NUM_KNIGHTS; i++) {
        move_list_t *knight_moves = generate_knight_moves(game->board, knights[i]);
        add_all(master_list, knight_moves);
    }

    // bishops
    for (int i = 0; i < NUM_BISHOPS; i++) {
        move_list_t *bishop_moves = generate_bishop_moves(game->board, bishops[i]);
        add_all(master_list, bishop_moves);
    }

    // queens
    for (int i = 0; i < NUM_QUEENS; i++) {
        move_list_t *queen_moves = generate_queen_moves(game->board, queens[i]);
        add_all(master_list, queen_moves);
    }

    // king
    move_list_t *king_moves = generate_king_moves(game->board, king);
    add_all(master_list, king_moves);

    // kingside castle
    move_list_t *kingside_castle_moves = generate_castling_moves(game, king, rooks[1]);
    add_all(master_list, kingside_castle_moves);

    // queenside castle
    move_list_t *queenside_castle_moves = generate_castling_moves(game, king, rooks[0]);
    add_all(master_list, queenside_castle_moves);
    
    return master_list;
}