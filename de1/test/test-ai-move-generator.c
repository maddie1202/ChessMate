#include "include/test-ai-move-generator.h"
#include "../include/game.h"
#include "../include/possible-move-generators.h"
#include "include/test.h"
#include "../include/ai-move-generator.h"
#include <stdlib.h>
#include <stdio.h>

#define PRINT_BOARDS 0
#define MAX_MOVES 100
#define MINIMAX_DEPTH 3

typedef struct move 
{
    char piece;
    int x;
    int y;
    bool castle;
} move_t;

move_t move(char piece, int x, int y, bool castle)
{
    move_t move;

    move.piece = piece;
    move.x = x;
    move.y = y;
    move.castle = castle;

    return move;
}

void test_ai_move_generator()
{
    test_in_check();
    test_in_checkmate();
    test_generate_ai_move();
}

move_t moves[MAX_MOVES];
int num_moves = 0;

static void add_move(move_t move)
{
    moves[num_moves++] = move;
}

static void initialize_moves()
{
    add_move(move(WPAWN4, 4, 3, false));
    add_move(move(BKNIGHT1, 5, 5, false));
    add_move(move(WKNIGHT0, 2, 2, false));
    add_move(move(BKNIGHT0, 2, 5, false));
    add_move(move(WBISHOP1, 2, 3, false));
    add_move(move(BKNIGHT0, 4, 4, false));
    add_move(move(WPAWN1, 1, 2, false));
    add_move(move(BPAWN4, 4, 5, false));
    add_move(move(WKNIGHT1, 5, 2, false));
    add_move(move(BKNIGHT0, 2, 3, false));
    add_move(move(WPAWN1, 2, 3, false));
    add_move(move(BBISHOP1, 1, 3, false));
    add_move(move(WKING, 6, 0, true));
    add_move(move(BPAWN3, 3, 4, false));
    add_move(move(WKNIGHT1, 4, 4, false));
    add_move(move(BPAWN3 , 2, 3, false));
    add_move(move(WPAWN0 , 0, 1, false));
    add_move(move(BBISHOP1, 2, 2, false));
    add_move(move(WPAWN3, 2, 2, false));
    add_move(move(BKNIGHT1, 4, 3, false));
    add_move(move(WROOK1, 4, 0, false));
    add_move(move(BKNIGHT1, 2, 2, false));
    add_move(move(WQUEEN0, 3, 1, false));
    add_move(move(BKNIGHT1, 4, 3, false));
    add_move(move(WROOK1, 4, 3, false));
    add_move(move(BQUEEN0, 5, 5, false));
    add_move(move(WQUEEN0, 5, 3, false));
    add_move(move(BQUEEN0, 5, 4, false));
    add_move(move(WQUEEN0, 5, 4, false));
    add_move(move(BPAWN4, 5, 4, false));
    add_move(move(WBISHOP0, 6, 4, false));
    add_move(move(BPAWN4, 4, 3, false));
    add_move(move(WROOK0, 3, 0, false));
    add_move(move(BPAWN5, 5, 5, false));
    add_move(move(WBISHOP0, 5, 5, false));
    add_move(move(BPAWN6, 5, 5, false));
    add_move(move(WKNIGHT1, 3, 6, false));
    add_move(move(BBISHOP0, 3, 6, false));
    add_move(move(WROOK0, 3, 6, false));
    add_move(move(BKING, 3, 6, false));
    add_move(move(WPAWN0, 0, 3, false));
    add_move(move(BROOK1, 6, 7, false));
    add_move(move(WPAWN0, 0, 4, false));
    add_move(move(BKING, 2, 7, false));
    add_move(move(WPAWN0, 0, 5, false));
    add_move(move(BPAWN1, 0, 5, false));
    add_move(move(WPAWN2, 2, 2, false));
    add_move(move(BKING, 1, 7, false));
    add_move(move(WKING, 7, 0, false));
    add_move(move(BPAWN6, 5, 4, false));
    add_move(move(WPAWN5, 5, 2, false));
    add_move(move(BPAWN4, 4, 2, false));
    add_move(move(WPAWN5, 5, 3, false));
    add_move(move(BPAWN4, 4, 1, false));
    add_move(move(WKING, 6, 0, false));
    add_move(move(BPAWN4, 4, 0, false)); // pawn promotion not working here
}

static void move_rook_for_castle(game_t *game, move_t move, int colour)
{
    if (!move.castle || !is_king(move.piece)) return;

    if (colour == WHITE && move.x == 6) {
        move_piece(game->board, WROOK1, 5, 0);
    } else if (colour == WHITE && move.x == 2) {
        move_piece(game->board, WROOK0, 3, 0);
    } else if (colour == BLACK && move.x == 6) {
        move_piece(game->board, BROOK1, 5, 7);
    } else if (colour == BLACK && move.x == 2) {
        move_piece(game->board, BROOK0, 3, 7);
    }
}

// requires wmove.piece to be white and bmove.piece to be black
static bool test_move_and_response(game_t *game, move_t wmove, move_t bmove)
{
    move_piece(game->board, wmove.piece, wmove.x, wmove.y);
    move_rook_for_castle(game, wmove, WHITE);

    if (PRINT_BOARDS) print_board(game->board);

    board_t *actual = generate_ai_move(game, BLACK, MINIMAX_DEPTH);

    move_piece(game->board, bmove.piece, bmove.x, bmove.y);
    move_rook_for_castle(game, bmove, BLACK);
    
    if (PRINT_BOARDS) print_board(game->board);

    double expected_score = eval_board(game->board, BLACK);
    double actual_score = eval_board(actual, BLACK);

    if (actual != NULL) free(actual);

    if (expected_score == actual_score) {
        print_test_result(test_result(true, ""), __func__);
        return true;
    } else {
        print_test_result(test_result(false, "Unequal scores"), __func__);
        return false;
    }
}

void test_generate_ai_move()
{
    game_t *game = init_game();
    initialize_moves();
    bool passing = true;

    for (int i = 0; i < num_moves - 1 && passing; i += 2) {
        test_move_and_response(game, moves[i], moves[i + 1]);
    }
}

static void test_in_check0w()
{
    board_t board = {
            {WROOK0,   WKNIGHT0, EMPTY,  WQUEEN0,   EMPTY,    EMPTY,    WKNIGHT1, WROOK1},
            {WPAWN0,   EMPTY,    EMPTY,  WPAWN3,   EMPTY,    EMPTY,    WPAWN6,   WPAWN7},       
            {WBISHOP0, WPAWN1,   WPAWN2, WBISHOP1, EMPTY,    WPAWN5,   EMPTY,    EMPTY},
            {EMPTY,    EMPTY,    EMPTY,  EMPTY,    WPAWN4,   WKING,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,    BKING,  BPAWN3,   EMPTY,    EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,    BPAWN2, EMPTY,    BBISHOP0, BPAWN5,   BPAWN6,   BBISHOP1},
            {BPAWN0,   BPAWN1,   EMPTY,  EMPTY,    BPAWN4,   EMPTY,    EMPTY,    BPAWN7},
            {BROOK0,   BKNIGHT0, EMPTY,  BQUEEN0,   EMPTY,    EMPTY,    BKNIGHT1, BROOK1}
        };
    
    bool actual = in_check(&board, WHITE);

    print_test_result(test_result(actual == true, ""), __func__);
}

static void test_in_check0b()
{
    board_t board = {
            {WROOK0,   WKNIGHT0, EMPTY,  WQUEEN0,   EMPTY,    EMPTY,    WKNIGHT1, WROOK1},
            {WPAWN0,   EMPTY,    EMPTY,  WPAWN3,   EMPTY,    EMPTY,    WPAWN6,   WPAWN7},       
            {WBISHOP0, WPAWN1,   WPAWN2, WBISHOP1, EMPTY,    WPAWN5,   EMPTY,    EMPTY},
            {EMPTY,    EMPTY,    EMPTY,  EMPTY,    WPAWN4,   WKING,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,    BKING,  BPAWN3,   EMPTY,    EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,    BPAWN2, EMPTY,    BBISHOP0, BPAWN5,   BPAWN6,   BBISHOP1},
            {BPAWN0,   BPAWN1,   EMPTY,  EMPTY,    BPAWN4,   EMPTY,    EMPTY,    BPAWN7},
            {BROOK0,   BKNIGHT0, EMPTY,  BQUEEN0,   EMPTY,    EMPTY,    BKNIGHT1, BROOK1}
        };
    
    bool actual = in_check(&board, BLACK);

    print_test_result(test_result(actual == true, ""), __func__);
}

static void test_in_check1w()
{
    board_t board = {
            {WROOK0,   WKNIGHT0, EMPTY,  WQUEEN0,   EMPTY,    EMPTY,    WKNIGHT1, WROOK1},
            {WPAWN0,   EMPTY,    EMPTY,  WPAWN3,   EMPTY,    EMPTY,    EMPTY,    WPAWN7},       
            {WBISHOP0, WPAWN1,   WPAWN2, WBISHOP1, EMPTY,    WPAWN5,   WPAWN6,   EMPTY},
            {EMPTY,    EMPTY,    EMPTY,  EMPTY,    WPAWN4,   WKING,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,    BKING,  BPAWN3,   EMPTY,    EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    BPAWN1,   BPAWN2, EMPTY,    BBISHOP0, BPAWN5,   BPAWN6,   BBISHOP1},
            {BPAWN0,   EMPTY,    EMPTY,  EMPTY,    BPAWN4,   EMPTY,    EMPTY,    BPAWN7},
            {BROOK0,   BKNIGHT0, EMPTY,  BQUEEN0,   EMPTY,    EMPTY,    BKNIGHT1, BROOK1}
        };

    bool actual = in_check(&board, WHITE);

    print_test_result(test_result(actual == true, ""), __func__);
}

static void test_in_check1b()
{
    board_t board = {
            {WROOK0,   WKNIGHT0, EMPTY,  WQUEEN0,   EMPTY,    EMPTY,    WKNIGHT1, WROOK1},
            {WPAWN0,   EMPTY,    EMPTY,  WPAWN3,   EMPTY,    EMPTY,    EMPTY,    WPAWN7},       
            {WBISHOP0, WPAWN1,   WPAWN2, WBISHOP1, EMPTY,    WPAWN5,   WPAWN6,   EMPTY},
            {EMPTY,    EMPTY,    EMPTY,  EMPTY,    WPAWN4,   WKING,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,    BKING,  BPAWN3,   EMPTY,    EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    BPAWN1,   BPAWN2, EMPTY,    BBISHOP0, BPAWN5,   BPAWN6,   BBISHOP1},
            {BPAWN0,   EMPTY,    EMPTY,  EMPTY,    BPAWN4,   EMPTY,    EMPTY,    BPAWN7},
            {BROOK0,   BKNIGHT0, EMPTY,  BQUEEN0,   EMPTY,    EMPTY,    BKNIGHT1, BROOK1}
        };

    bool actual = in_check(&board, BLACK);

    print_test_result(test_result(actual == true, ""), __func__);
}

static void test_in_check2w()
{
    board_t *board = init_board();
    bool actual = in_check(board, WHITE);

    print_test_result(test_result(actual == false, ""), __func__);
    
    free(board);
}

static void test_in_check2b()
{
    board_t *board = init_board();
    bool actual = in_check(board, BLACK);

    print_test_result(test_result(actual == false, ""), __func__);
    
    free(board);
}

static void test_in_check3w()
{
    board_t board = {
            {WROOK0, EMPTY,  WBISHOP0, EMPTY,    EMPTY,   EMPTY,    EMPTY,  WKING},
            {WPAWN0, WPAWN1, WPAWN2,   WPAWN3,   EMPTY,   EMPTY,    BPAWN6, WPAWN7},       
            {EMPTY,  EMPTY,  WKNIGHT0, EMPTY,    WQUEEN0, BQUEEN0,  EMPTY,  EMPTY},
            {EMPTY,  EMPTY,  WBISHOP1, EMPTY,    WPAWN4,  EMPTY,    EMPTY,  EMPTY},
            {EMPTY,  EMPTY,  BBISHOP1, EMPTY,    BPAWN4,  EMPTY,    EMPTY,  BKNIGHT1},
            {EMPTY,  EMPTY,  BKNIGHT0, BPAWN3,   EMPTY,   WKNIGHT1, EMPTY,  BPAWN7},
            {BPAWN0, BPAWN1, BPAWN2,   BBISHOP0, BROOK1,  WROOK1,   EMPTY,  EMPTY},
            {BROOK0, EMPTY,  EMPTY,    EMPTY,    EMPTY,   EMPTY,    EMPTY,  BKING},
        };

    bool actual = in_check(&board, WHITE);

    print_test_result(test_result(actual == true, ""), __func__);
}

static void test_in_check3b()
{
    board_t board = {
            {WROOK0, EMPTY,  WBISHOP0, EMPTY,    EMPTY,   EMPTY,    EMPTY,  WKING},
            {WPAWN0, WPAWN1, WPAWN2,   WPAWN3,   EMPTY,   EMPTY,    BPAWN6, WPAWN7},       
            {EMPTY,  EMPTY,  WKNIGHT0, EMPTY,    WQUEEN0, BQUEEN0,  EMPTY,  EMPTY},
            {EMPTY,  EMPTY,  WBISHOP1, EMPTY,    WPAWN4,  EMPTY,    EMPTY,  EMPTY},
            {EMPTY,  EMPTY,  BBISHOP1, EMPTY,    BPAWN4,  EMPTY,    EMPTY,  BKNIGHT1},
            {EMPTY,  EMPTY,  BKNIGHT0, BPAWN3,   EMPTY,   WKNIGHT1, EMPTY,  BPAWN7},
            {BPAWN0, BPAWN1, BPAWN2,   BBISHOP0, BROOK1,  WROOK1,   EMPTY,  EMPTY},
            {BROOK0, EMPTY,  EMPTY,    EMPTY,    EMPTY,   EMPTY,    EMPTY,  BKING},
        };

    bool actual = in_check(&board, BLACK);

    print_test_result(test_result(actual == false, ""), __func__);
}

static void test_in_check4w()
{
    board_t board = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  WQUEEN0,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, EMPTY, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    bool actual = in_check(&board, WHITE);

    print_test_result(test_result(actual == false, ""), __func__);
}

static void test_in_check4b()
{
    board_t board = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  WQUEEN0,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, EMPTY, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    bool actual = in_check(&board, BLACK);

    print_test_result(test_result(actual == true, ""), __func__);
}

static void test_in_check5w()
{
    board_t board = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, WQUEEN0, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    bool actual = in_check(&board, WHITE);

    print_test_result(test_result(actual == false, ""), __func__);
}

static void test_in_check5b()
{
    board_t board = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, WQUEEN0, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    bool actual = in_check(&board, BLACK);

    print_test_result(test_result(actual == true, ""), __func__);
}

static void test_in_check6w()
{
    board_t board = {
            {WROOK0, WKNIGHT0, EMPTY, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WBISHOP0,   WPAWN2,   WPAWN3, WPAWN4, BQUEEN0,   WPAWN6,   WPAWN7},       
            {WPAWN0,  WPAWN1,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    BBISHOP1,    EMPTY,  BPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, EMPTY, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, EMPTY, BKING,  EMPTY, BKNIGHT1, BROOK1},
        };

    bool actual = in_check(&board, WHITE);

    print_test_result(test_result(actual == true, ""), __func__);
}

static void test_in_check6b()
{
    board_t board = {
            {WROOK0, WKNIGHT0, EMPTY, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WBISHOP0,   WPAWN2,   WPAWN3, WPAWN4, BQUEEN0,   WPAWN6,   WPAWN7},       
            {WPAWN0,  WPAWN1,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    BBISHOP1,    EMPTY,  BPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, EMPTY, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, EMPTY, BKING,  EMPTY, BKNIGHT1, BROOK1},
        };

    bool actual = in_check(&board, BLACK);

    print_test_result(test_result(actual == false, ""), __func__);
}

void test_in_check()
{
    test_in_check0w();
    test_in_check0b();
    test_in_check1w();
    test_in_check1b();
    test_in_check2w();
    test_in_check2b();
    test_in_check3w();
    test_in_check3b();
    test_in_check4w();
    test_in_check4b();
    test_in_check5w();
    test_in_check5b();
    test_in_check6w();
    test_in_check6b();
}

static void test_in_checkmate0w()
{
    board_t board = {
            {WROOK0,   WKNIGHT0, EMPTY,  WQUEEN0,   EMPTY,    EMPTY,    WKNIGHT1, WROOK1},
            {WPAWN0,   EMPTY,    EMPTY,  WPAWN3,   EMPTY,    EMPTY,    WPAWN6,   WPAWN7},       
            {WBISHOP0, WPAWN1,   WPAWN2, WBISHOP1, EMPTY,    WPAWN5,   EMPTY,    EMPTY},
            {EMPTY,    EMPTY,    EMPTY,  EMPTY,    WPAWN4,   WKING,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,    BKING,  BPAWN3,   EMPTY,    EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,    BPAWN2, EMPTY,    BBISHOP0, BPAWN5,   BPAWN6,   BBISHOP1},
            {BPAWN0,   BPAWN1,   EMPTY,  EMPTY,    BPAWN4,   EMPTY,    EMPTY,    BPAWN7},
            {BROOK0,   BKNIGHT0, EMPTY,  BQUEEN0,   EMPTY,    EMPTY,    BKNIGHT1, BROOK1}
        };
    
    bool actual = in_checkmate(&board, WHITE);

    print_test_result(test_result(actual == false, ""), __func__);
}

static void test_in_checkmate0b()
{
    board_t board = {
            {WROOK0,   WKNIGHT0, EMPTY,  WQUEEN0,   EMPTY,    EMPTY,    WKNIGHT1, WROOK1},
            {WPAWN0,   EMPTY,    EMPTY,  WPAWN3,   EMPTY,    EMPTY,    WPAWN6,   WPAWN7},       
            {WBISHOP0, WPAWN1,   WPAWN2, WBISHOP1, EMPTY,    WPAWN5,   EMPTY,    EMPTY},
            {EMPTY,    EMPTY,    EMPTY,  EMPTY,    WPAWN4,   WKING,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,    BKING,  BPAWN3,   EMPTY,    EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,    BPAWN2, EMPTY,    BBISHOP0, BPAWN5,   BPAWN6,   BBISHOP1},
            {BPAWN0,   BPAWN1,   EMPTY,  EMPTY,    BPAWN4,   EMPTY,    EMPTY,    BPAWN7},
            {BROOK0,   BKNIGHT0, EMPTY,  BQUEEN0,   EMPTY,    EMPTY,    BKNIGHT1, BROOK1}
        };
    
    bool actual = in_checkmate(&board, BLACK);

    print_test_result(test_result(actual == false, ""), __func__);
}

static void test_in_checkmate1w()
{
    board_t board = {
            {WROOK0,   WKNIGHT0, EMPTY,  WQUEEN0,   EMPTY,    EMPTY,    WKNIGHT1, WROOK1},
            {WPAWN0,   EMPTY,    EMPTY,  WPAWN3,   EMPTY,    EMPTY,    EMPTY,    WPAWN7},       
            {WBISHOP0, WPAWN1,   WPAWN2, WBISHOP1, EMPTY,    WPAWN5,   WPAWN6,   EMPTY},
            {EMPTY,    EMPTY,    EMPTY,  EMPTY,    WPAWN4,   WKING,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,    BKING,  BPAWN3,   EMPTY,    EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    BPAWN1,   BPAWN2, EMPTY,    BBISHOP0, BPAWN5,   BPAWN6,   BBISHOP1},
            {BPAWN0,   EMPTY,    EMPTY,  EMPTY,    BPAWN4,   EMPTY,    EMPTY,    BPAWN7},
            {BROOK0,   BKNIGHT0, EMPTY,  BQUEEN0,   EMPTY,    EMPTY,    BKNIGHT1, BROOK1}
        };

    bool actual = in_checkmate(&board, WHITE);

    print_test_result(test_result(actual == true, ""), __func__);
}

static void test_in_checkmate1b()
{
    board_t board = {
            {WROOK0,   WKNIGHT0, EMPTY,  WQUEEN0,   EMPTY,    EMPTY,    WKNIGHT1, WROOK1},
            {WPAWN0,   EMPTY,    EMPTY,  WPAWN3,   EMPTY,    EMPTY,    EMPTY,    WPAWN7},       
            {WBISHOP0, WPAWN1,   WPAWN2, WBISHOP1, EMPTY,    WPAWN5,   WPAWN6,   EMPTY},
            {EMPTY,    EMPTY,    EMPTY,  EMPTY,    WPAWN4,   WKING,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,    BKING,  BPAWN3,   EMPTY,    EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    BPAWN1,   BPAWN2, EMPTY,    BBISHOP0, BPAWN5,   BPAWN6,   BBISHOP1},
            {BPAWN0,   EMPTY,    EMPTY,  EMPTY,    BPAWN4,   EMPTY,    EMPTY,    BPAWN7},
            {BROOK0,   BKNIGHT0, EMPTY,  BQUEEN0,   EMPTY,    EMPTY,    BKNIGHT1, BROOK1}
        };

    bool actual = in_checkmate(&board, BLACK);

    print_test_result(test_result(actual == true, ""), __func__);
}

static void test_in_checkmate2w()
{
    board_t *board = init_board();
    bool actual = in_checkmate(board, WHITE);

    print_test_result(test_result(actual == false, ""), __func__);
    
    free(board);
}

static void test_in_checkmate2b()
{
    board_t *board = init_board();
    bool actual = in_checkmate(board, BLACK);

    print_test_result(test_result(actual == false, ""), __func__);
    
    free(board);
}

static void test_in_checkmate3w()
{
    board_t board = {
            {WROOK0, EMPTY,  WBISHOP0, EMPTY,    EMPTY,   EMPTY,    EMPTY,  WKING},
            {WPAWN0, WPAWN1, WPAWN2,   WPAWN3,   EMPTY,   EMPTY,    BPAWN6, WPAWN7},       
            {EMPTY,  EMPTY,  WKNIGHT0, EMPTY,    WQUEEN0, BQUEEN0,  EMPTY,  EMPTY},
            {EMPTY,  EMPTY,  WBISHOP1, EMPTY,    WPAWN4,  EMPTY,    EMPTY,  EMPTY},
            {EMPTY,  EMPTY,  BBISHOP1, EMPTY,    BPAWN4,  EMPTY,    EMPTY,  BKNIGHT1},
            {EMPTY,  EMPTY,  BKNIGHT0, BPAWN3,   EMPTY,   WKNIGHT1, EMPTY,  BPAWN7},
            {BPAWN0, BPAWN1, BPAWN2,   BBISHOP0, BROOK1,  WROOK1,   EMPTY,  EMPTY},
            {BROOK0, EMPTY,  EMPTY,    EMPTY,    EMPTY,   EMPTY,    EMPTY,  BKING},
        };

    bool actual = in_checkmate(&board, WHITE);

    print_test_result(test_result(actual == false, ""), __func__);
}

static void test_in_checkmate3b()
{
    board_t board = {
            {WROOK0, EMPTY,  WBISHOP0, EMPTY,    EMPTY,   EMPTY,    EMPTY,  WKING},
            {WPAWN0, WPAWN1, WPAWN2,   WPAWN3,   EMPTY,   EMPTY,    BPAWN6, WPAWN7},       
            {EMPTY,  EMPTY,  WKNIGHT0, EMPTY,    WQUEEN0, BQUEEN0,  EMPTY,  EMPTY},
            {EMPTY,  EMPTY,  WBISHOP1, EMPTY,    WPAWN4,  EMPTY,    EMPTY,  EMPTY},
            {EMPTY,  EMPTY,  BBISHOP1, EMPTY,    BPAWN4,  EMPTY,    EMPTY,  BKNIGHT1},
            {EMPTY,  EMPTY,  BKNIGHT0, BPAWN3,   EMPTY,   WKNIGHT1, EMPTY,  BPAWN7},
            {BPAWN0, BPAWN1, BPAWN2,   BBISHOP0, BROOK1,  WROOK1,   EMPTY,  EMPTY},
            {BROOK0, EMPTY,  EMPTY,    EMPTY,    EMPTY,   EMPTY,    EMPTY,  BKING},
        };

    bool actual = in_checkmate(&board, BLACK);

    print_test_result(test_result(actual == false, ""), __func__);
}

static void test_in_checkmate4w()
{
    board_t board = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  WQUEEN0,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, EMPTY, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    bool actual = in_checkmate(&board, WHITE);

    print_test_result(test_result(actual == false, ""), __func__);
}

static void test_in_checkmate4b()
{
    board_t board = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  WQUEEN0,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, EMPTY, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    bool actual = in_checkmate(&board, BLACK);

    print_test_result(test_result(actual == true, ""), __func__);
}

static void test_in_checkmate5w()
{
    board_t board = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, WQUEEN0, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    bool actual = in_checkmate(&board, WHITE);

    print_test_result(test_result(actual == false, ""), __func__);
}

static void test_in_checkmate5b()
{
    board_t board = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, WQUEEN0, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    bool actual = in_checkmate(&board, BLACK);

    print_test_result(test_result(actual == false, ""), __func__);
}

static void test_in_checkmate6w()
{
    board_t board = {
            {WROOK0, WKNIGHT0, EMPTY, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WBISHOP0,   WPAWN2,   WPAWN3, WPAWN4, BQUEEN0,   WPAWN6,   WPAWN7},       
            {WPAWN0,  WPAWN1,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    BBISHOP1,    EMPTY,  BPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, EMPTY, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, EMPTY, BKING,  EMPTY, BKNIGHT1, BROOK1},
        };

    bool actual = in_checkmate(&board, WHITE);

    print_test_result(test_result(actual == true, ""), __func__);
}

static void test_in_checkmate6b()
{
    board_t board = {
            {WROOK0, WKNIGHT0, EMPTY, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WBISHOP0,   WPAWN2,   WPAWN3, WPAWN4, BQUEEN0,   WPAWN6,   WPAWN7},       
            {WPAWN0,  WPAWN1,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    BBISHOP1,    EMPTY,  BPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, EMPTY, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, EMPTY, BKING,  EMPTY, BKNIGHT1, BROOK1},
        };

    bool actual = in_checkmate(&board, BLACK);

    print_test_result(test_result(actual == false, ""), __func__);
}

void test_in_checkmate()
{
    test_in_checkmate0w();
    test_in_checkmate0b();
    test_in_checkmate1w();
    test_in_checkmate1b();
    test_in_checkmate2w();
    test_in_checkmate2b();
    test_in_checkmate3w();
    test_in_checkmate3b();
    test_in_checkmate4w();
    test_in_checkmate4b();
    test_in_checkmate5w();
    test_in_checkmate5b();
    test_in_checkmate6w();
    test_in_checkmate6b();
}