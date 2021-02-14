#include "../../include/game.h"
#include "../../include/possible-move-generators.h"
#include "../include/test.h"
#include <stdlib.h>
#include <stdio.h>

static void rook_test0w()
{
    board_t *board = init_board();

    move_list_t exptected;
    exptected.moves = NULL;
    exptected.num_moves = 0;

    move_list_t *actual = generate_rook_moves(board, WROOK0);

    test_result_t result = board_list_equals(&exptected, actual);
    print_test_result(result, __func__);

    free(board);
    free(actual);
}

static void rook_test0b()
{
    board_t *board = init_board();

    move_list_t exptected;
    exptected.moves = NULL;
    exptected.num_moves = 0;

    move_list_t *actual = generate_rook_moves(board, BROOK1);

    test_result_t result = board_list_equals(&exptected, actual);
    print_test_result(result, __func__);

    free(board);
    free(actual);
}

static void rook_test1w()
{
    board_t board = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    WROOK0,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected1 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    WROOK0,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected2 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    WROOK0,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected3 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    WROOK0,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };
    
    board_t expected4 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   WROOK0, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected5 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    WROOK0,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected6 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, WROOK0,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected7 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WROOK0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected8 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WROOK0,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected9 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  WROOK0,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected10 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    WROOK0,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected11 = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    WROOK0},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    move_list_t expected;
    board_t* expected_moves[11] = {&expected1, &expected2, &expected3, &expected4, &expected5, &expected6, 
        &expected7, &expected8, &expected9, &expected10, &expected11};
    expected.moves = expected_moves;
    expected.num_moves = 2;

    move_list_t *actual = generate_rook_moves(&board, WROOK0);
    test_result_t result = board_list_equals(&expected, actual);
    print_test_result(result, __func__);
}

static void rook_test1b()
{
    board_t board = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    BPAWN7},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    BROOK1},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   EMPTY},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, EMPTY},
        };
    
    board_t expected1 = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    BPAWN7},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    BROOK1},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   EMPTY},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, EMPTY},
        };

    board_t expected2 = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    BPAWN7},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BROOK1},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, EMPTY},
        };

    board_t expected3 = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    BPAWN7},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   EMPTY},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t expected4 = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    BPAWN7},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    BROOK1,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   EMPTY},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, EMPTY},
        };

    board_t expected5 = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    BPAWN7},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  BROOK1,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   EMPTY},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, EMPTY},
        };

    board_t expected6 = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    BPAWN7},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  BROOK1,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   EMPTY},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, EMPTY},
        };

    board_t expected7 = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    BPAWN7},
            {EMPTY, EMPTY,    EMPTY,    BROOK1,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   EMPTY},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, EMPTY},
        };

    board_t expected8 = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    BPAWN7},
            {EMPTY, EMPTY,    BROOK1,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   EMPTY},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, EMPTY},
        };

    board_t expected9 = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    BPAWN7},
            {EMPTY, BROOK1,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   EMPTY},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, EMPTY},
        };

    board_t expected10 = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    BPAWN7},
            {BROOK1, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   EMPTY},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, EMPTY},
        };

    move_list_t expected;
    board_t* expected_moves[11] = {&expected1, &expected2, &expected3, &expected4, &expected5, &expected6, 
        &expected7, &expected8, &expected9, &expected10};
    expected.moves = expected_moves;
    expected.num_moves = 2;

    move_list_t *actual = generate_rook_moves(&board, WROOK0);
    test_result_t result = board_list_equals(&expected, actual);
    print_test_result(result, __func__);
}

void test_rook_generator()
{
    rook_test0w();
    rook_test0b();
    rook_test1w();
    rook_test1b();
}

