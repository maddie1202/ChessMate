#ifndef GAME
#define GAME

typedef char board_t[8][8];

// for pawn upgrade: use upper #s (tbd)
#define WPAWN0   0
#define WPAWN1   1
#define WPAWN2   2
#define WPAWN3   3
#define WPAWN4   4
#define WPAWN5   5
#define WPAWN6   6
#define WPAWN7   7
#define WROOK0   8
#define WROOK1   9
#define WKNIGHT0 10
#define WKNIGHT1 11
#define WBISHOP0 12
#define WBISHOP1 13
#define WQUEEN   14
#define WKING    15

#define BPAWN0   16
#define BPAWN1   17
#define BPAWN2   18
#define BPAWN3   19
#define BPAWN4   20
#define BPAWN5   21
#define BPAWN6   22
#define BPAWN7   23
#define BROOK0   24
#define BROOK1   25
#define BKNIGHT0 26
#define BKNIGHT1 27
#define BBISHOP0 28
#define BBISHOP1 29
#define BQUEEN   30
#define BKING    31

#define EMPTY    32

board_t* current_game;

board_t* init_board();
void init_and_set_board();
board_t* copy_board(board_t* original);
void destroy_board();

int find_piece(board_t *board, char piece, int *x, int *y);
void move_piece(board_t *board, char piece, int dest_x, int dest_y);

void print_board(board_t *board);

#endif


