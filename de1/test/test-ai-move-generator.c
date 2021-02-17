#include "include/test-ai-move-generator.h"
#include "../include/game.h"
#include "../include/possible-move-generators.h"
#include "include/test.h"
#include "../include/ai-move-generator.h"
#include <stdlib.h>
#include <stdio.h>

void test_ai_move_generator()
{
    test_in_check();
    test_in_checkmate();
    test_generate_ai_move();
}

static void test_generate_ai_move0()
{
    game_t *game = init_game();
    board_t board = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    free(game->board);
    game->board = &board;

    board_t expected = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  BKNIGHT1, EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, EMPTY, BROOK1},
        };

    board_t *actual = generate_ai_move(game, BLACK, 3);

    double expected_score = eval_board(&expected, BLACK);
    double actual_score = eval_board(actual, BLACK);

    if (expected_score == actual_score) {
        print_test_result(test_result(true, ""), __func__);
    } else {
        print_test_result(test_result(false, "Unequal scores"), __func__);
    }
}

void test_generate_ai_move()
{
    test_generate_ai_move0();
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

    bool actual = in_check(&board, WHITE);

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

    print_test_result(test_result(actual == false, ""), __func__);
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

    bool actual = in_checkmate(&board, WHITE);

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