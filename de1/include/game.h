#ifndef GAME
#define GAME

#include <stdbool.h>

typedef char board_t[8][8];

typedef struct game
{
    board_t *board;
    bool wking_has_moved;
    bool wrook0_has_moved;
    bool wrook1_has_moved;
    bool bking_has_moved;
    bool brook0_has_moved;
    bool brook1_has_moved;
} game_t;

// for pawn upgrade: use upper #s (tbd)
#define WPAWN0  1
#define WPAWN1  2
#define WPAWN2  3
#define WPAWN3  4
#define WPAWN4  5
#define WPAWN5  6
#define WPAWN6  7
#define WPAWN7  8
#define WROOK0  9
#define WROOK1  10
#define WROOK2  11
#define WROOK3  12
#define WROOK4  13
#define WROOK5  14
#define WROOK6  15
#define WROOK7  16
#define WROOK8  17
#define WROOK9  18
#define WKNIGHT0  19
#define WKNIGHT1  20
#define WKNIGHT2  21
#define WKNIGHT3  22
#define WKNIGHT4  23
#define WKNIGHT5  24
#define WKNIGHT6  25
#define WKNIGHT7  26
#define WKNIGHT8  27
#define WKNIGHT9  28
#define WBISHOP0  29
#define WBISHOP1  30
#define WBISHOP2  31
#define WBISHOP3  32
#define WBISHOP4  33
#define WBISHOP5  34
#define WBISHOP6  35
#define WBISHOP7  36
#define WBISHOP8  37
#define WBISHOP9  38
#define WQUEEN0  39
#define WQUEEN1  40
#define WQUEEN2  41
#define WQUEEN3  42
#define WQUEEN4  43
#define WQUEEN5  44
#define WQUEEN6  45
#define WQUEEN7  46
#define WQUEEN8  47
#define WKING    48

#define BPAWN0  -1
#define BPAWN1  -2
#define BPAWN2  -3
#define BPAWN3  -4
#define BPAWN4  -5
#define BPAWN5  -6
#define BPAWN6  -7
#define BPAWN7  -8
#define BROOK0  -9
#define BROOK1  -10
#define BROOK2  -11
#define BROOK3  -12
#define BROOK4  -13
#define BROOK5  -14
#define BROOK6  -15
#define BROOK7  -16
#define BROOK8  -17
#define BROOK9  -18
#define BKNIGHT0  -19
#define BKNIGHT1  -20
#define BKNIGHT2  -21
#define BKNIGHT3  -22
#define BKNIGHT4  -23
#define BKNIGHT5  -24
#define BKNIGHT6  -25
#define BKNIGHT7  -26
#define BKNIGHT8  -27
#define BKNIGHT9  -28
#define BBISHOP0  -29
#define BBISHOP1  -30
#define BBISHOP2  -31
#define BBISHOP3  -32
#define BBISHOP4  -33
#define BBISHOP5  -34
#define BBISHOP6  -35
#define BBISHOP7  -36
#define BBISHOP8  -37
#define BBISHOP9  -38
#define BQUEEN0  -39
#define BQUEEN1  -40
#define BQUEEN2  -41
#define BQUEEN3  -42
#define BQUEEN4  -43
#define BQUEEN5  -44
#define BQUEEN6  -45
#define BQUEEN7  -46
#define BQUEEN8  -47
#define BKING    -48

#define EMPTY    0

#define OUT_OF_BOUNDS 50

#define NUM_PAWNS 8
#define NUM_ROOKS 10 
#define NUM_KNIGHTS 10 
#define NUM_BISHOPS 10 
#define NUM_QUEENS 9

extern char wpawns[NUM_PAWNS];
extern char bpawns[NUM_PAWNS];

extern char wrooks[NUM_ROOKS];
extern char brooks[NUM_ROOKS];

extern char wknights[NUM_KNIGHTS];
extern char bknights[NUM_KNIGHTS];

extern char wbishops[NUM_BISHOPS];
extern char bbishops[NUM_BISHOPS];

extern char wqueens[NUM_QUEENS];
extern char bqueens[NUM_QUEENS];

game_t* current_game;

board_t* init_board();
void init_and_set_board();
board_t* copy_board(board_t* original);
void destroy_board();
game_t *init_game();

int find_piece(board_t *board, char piece, int *x, int *y);
void move_piece(board_t *board, char piece, int dest_x, int dest_y);
char get_piece(board_t *board, int x, int y);   

void print_board(board_t *board);

bool is_pawn(char piece);
bool is_knight(char piece);
bool is_king(char piece);
bool is_bishop(char piece);
bool is_rook(char piece);
bool is_queen(char piece);

bool has_king_moved(game_t *game, char king);
bool has_rook_moved(game_t *game, char rook);

#endif


