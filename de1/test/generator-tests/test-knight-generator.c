#include "../../include/game.h"
#include "../../include/possible-move-generators.h"
#include "../include/test.h"
#include <stdlib.h>
#include <stdio.h>

/* 
 * This file contains 3 tests for the knight move generator.
 * TEST 0: Knight's first move
 * TEST 1: No possible moves/can't take pieces of the same colour as themselves
 * TEST 2: Take an enemy piece
 */

/* TEST 0: 
 * Test a knight straight from initial setup for both black and white 
 */
static void knight_test0()
{
    board_t* curr = init_board();

    // move WKNIGHT0 from (1, 0) to (0, 2)
    board_t* w_expected1 = copy_board(curr);
    move_piece(w_expected1, WKNIGHT0, 0, 2);

    // move WKNIGHT0 from (1, 0) to (2, 2)
    board_t* w_expected2 = copy_board(curr);
    move_piece(w_expected2, WKNIGHT0, 2, 2);

    move_list_t w_expected;
    board_t* w_expected_moves[2] = {w_expected1, w_expected2};
    w_expected.moves = w_expected_moves;
    w_expected.num_moves = 2;

    // generate WKNIGHT0 moves
    move_list_t *w_actual = generate_knight_moves(curr, WKNIGHT0);

    // move BKNIGHT0 from (1, 7) to (0, 5)
    board_t* b_expected1 = copy_board(curr);
    move_piece(b_expected1, BKNIGHT0, 0, 5);

    // move BKNIGHT0 from (1, 7) to (2, 5)
    board_t* b_expected2 = copy_board(curr);
    move_piece(b_expected2, BKNIGHT0, 2, 5);

    move_list_t b_expected;
    board_t* b_expected_moves[2] = {b_expected1, b_expected2};
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 2;

    // generate BKNIGHT0 moves
    move_list_t *b_actual = generate_knight_moves(curr, BKNIGHT0);
    
    // CMP boards
    test_result_t w_result = board_list_equals(&w_expected, w_actual);
    test_result_t b_result = board_list_equals(&b_expected, b_actual);

    // display results
    print_test_result(w_result, __func__);
    print_test_result(b_result, __func__);

    // free generated boards
    free(curr);
    free(w_expected1);
    free(w_expected2);
    free(b_expected1);
    free(b_expected2);
    free(w_actual);
    free(b_actual);
}

/* TEST 1: 
 * Knight has no possible moves, and cannot take pieces of the same colour as itself
 * Tests both black and white knights
 */
static void knight_test1()
{
    board_t curr = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, EMPTY,    WPAWN6,   EMPTY},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  WPAWN5,   EMPTY,    WPAWN7},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  BPAWN5,   EMPTY,    BPAWN7},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, EMPTY,    BPAWN6,   EMPTY},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1}
        };

    move_list_t w_expected;
    board_t* w_expected_moves[0];
    w_expected.moves = w_expected_moves;
    w_expected.num_moves = 0;

    // generate WKNIGHT1 moves
    move_list_t *w_actual = generate_knight_moves(&curr, WKNIGHT1);

    move_list_t b_expected;
    board_t* b_expected_moves[0];
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 0;

    // generate BKNIGHT1 moves
    move_list_t *b_actual = generate_knight_moves(&curr, BKNIGHT1);

    // CMP boards
    test_result_t w_result = board_list_equals(&w_expected, w_actual);
    test_result_t b_result = board_list_equals(&b_expected, b_actual);

    // display results
    print_test_result(w_result, __func__);
    print_test_result(b_result, __func__);

    // free generated boards
    free(w_actual);
    free(b_actual);
}

/* TEST 2:
 * Knight can take an enemy piece
 * Test both black and white knights
 */
static void knight_test2()
{
    board_t curr = {
            {WROOK0, EMPTY,  WBISHOP0, WQUEEN,   WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, EMPTY,  WPAWN2,   WPAWN3,   WPAWN4, EMPTY,    WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  WPAWN1, EMPTY,    BKNIGHT0, EMPTY,  WPAWN5,   EMPTY,    EMPTY},
            {EMPTY,  BPAWN1, EMPTY,    WKNIGHT0, EMPTY,  BPAWN5,   EMPTY,    EMPTY},
            {EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, EMPTY,  BPAWN2,   BPAWN3,   BPAWN4, EMPTY,    BPAWN6,   BPAWN7},
            {BROOK0, EMPTY,  BBISHOP0, BQUEEN,   BKING,  BBISHOP1, BKNIGHT1, BROOK1}
        };

    // move WKNIGHT0 from (3, 4) to (2, 2)
    board_t* w_expected1 = copy_board(&curr);
    move_piece(w_expected1, WKNIGHT0, 2, 2);

    // move WKNIGHT0 from (3, 4) to (4, 2)
    board_t* w_expected2 = copy_board(&curr);
    move_piece(w_expected2, WKNIGHT0, 4, 2);

    // move WKNIGHT0 from (3, 4) to (1, 5)
    board_t* w_expected3 = copy_board(&curr);
    move_piece(w_expected3, WKNIGHT0, 1, 5);

    // move WKNIGHT0 from (3, 4) to (5, 5)
    board_t* w_expected4 = copy_board(&curr);
    move_piece(w_expected4, WKNIGHT0, 5, 5);

    // move WKNIGHT0 from (3, 4) to (2, 6) and take BPAWN2
    board_t* w_expected5 = copy_board(&curr);
    move_piece(w_expected5, WKNIGHT0, 2, 6);

    // move WKNIGHT0 from (3, 4) to (4, 6) and take BPAWN4
    board_t* w_expected6 = copy_board(&curr);
    move_piece(w_expected6, WKNIGHT0, 4, 6);

    move_list_t w_expected;
    board_t* w_expected_moves[6] = {w_expected1, w_expected2, w_expected3, w_expected4, w_expected5, w_expected6};
    w_expected.moves = w_expected_moves;
    w_expected.num_moves = 6;

    // generate WKNIGHT0 moves
    move_list_t *w_actual = generate_knight_moves(&curr, WKNIGHT0);

    // move BKNIGHT0 from (3, 3) to (2, 5)
    board_t* b_expected1 = copy_board(&curr);
    move_piece(b_expected1, BKNIGHT0, 2, 5);

    // move BKNIGHT0 from (3, 3) to (2, 5)
    board_t* b_expected2 = copy_board(&curr);
    move_piece(b_expected2, BKNIGHT0, 2, 5);

    // move BKNIGHT0 from (3, 3) to (1, 2)
    board_t* b_expected3 = copy_board(&curr);
    move_piece(b_expected3, BKNIGHT0, 1, 2);

    // move BKNIGHT0 from (3, 3) to (5, 2)
    board_t* b_expected4 = copy_board(&curr);
    move_piece(b_expected4, BKNIGHT0, 5, 2);

    // move BKNIGHT0 from (3, 3) to (2, 1) and take WPAWN2
    board_t* b_expected5 = copy_board(&curr);
    move_piece(b_expected5, BKNIGHT0, 2, 1);

    // move BKNIGHT0 from (3, 3) to (4, 1) and take WPAWN4
    board_t* b_expected6 = copy_board(&curr);
    move_piece(b_expected6, BKNIGHT0, 4, 1);

    move_list_t b_expected;
    board_t* b_expected_moves[6] = {b_expected1, b_expected2, b_expected3, b_expected4, b_expected5, b_expected6};
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 6;

    // generate BKNIGHT0 moves
    move_list_t *b_actual = generate_knight_moves(&curr, BKNIGHT0);
    
    // CMP boards
    test_result_t w_result = board_list_equals(&w_expected, w_actual);
    test_result_t b_result = board_list_equals(&b_expected, b_actual);

    // display results
    print_test_result(w_result, __func__);
    print_test_result(b_result, __func__);

    // free generated boards
    free(w_expected1);
    free(w_expected2);
    free(w_expected3);
    free(w_expected4);
    free(w_expected5);
    free(w_expected6);
    free(b_expected1);
    free(b_expected2);
    free(b_expected3);
    free(b_expected4);
    free(b_expected5);
    free(b_expected6);
    free(w_actual);
    free(b_actual);
}

/*
 * Run the knight tests
 */
void test_knight_generator()
{
    knight_test0();
    knight_test1();
    knight_test2();
}