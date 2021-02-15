#include "../../include/game.h"
#include "../../include/possible-move-generators.h"
#include "../include/test.h"
#include <stdlib.h>
#include <stdio.h>

/* 
 * This file contains 5 tests for the pawn move generator.
 * TEST 0: Pawn's first move (can move ahead by either one or two spaces)
 * TEST 1: No possible moves
 * TEST 2: Take an enemy piece
 * TEST 3: Pawn can only move ahead by one space if it has already moved from its' original spot
 * TEST 4: Pawn cannot take a piece that is the same colour as itself
 */

/* TEST 0: 
 * Test a pawn straight from initial setup for both black and white 
 * Each pawn should have 2 options: move forward either 1 or 2 squares
 */
static void pawn_test0()
{
    board_t* curr = init_board();

    // move WPAWN2 from (2, 1) to (2, 2)
    board_t* w_expected1 = copy_board(curr);
    move_piece(w_expected1, WPAWN2, 2, 2);

    // move WPAWN2 from (2, 1) to (2, 3)
    board_t* w_expected2 = copy_board(curr);
    move_piece(w_expected2, WPAWN2, 2, 3);

    move_list_t w_expected;
    board_t* w_expected_moves[2] = {w_expected1, w_expected2};
    w_expected.moves = w_expected_moves;
    w_expected.num_moves = 2;

    // generate WPAWN2 moves
    move_list_t *w_actual = generate_pawn_moves(curr, WPAWN2);

    // move BPAWN2 from (2, 6) to (2, 5)
    board_t* b_expected1 = copy_board(curr);
    move_piece(b_expected1, BPAWN2, 2, 5);

    // move BPAWN2 from (2, 6) to (2, 4)
    board_t* b_expected2 = copy_board(curr);
    move_piece(b_expected2, BPAWN2, 2, 4);

    move_list_t b_expected;
    board_t* b_expected_moves[2] = {b_expected1, b_expected2};
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 2;

    // generate BPAWN2 moves
    move_list_t *b_actual = generate_pawn_moves(curr, BPAWN2);
    
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
    destroy_move_list(w_actual);
    destroy_move_list(b_actual);
}

/* TEST 1: 
 * Pawn4 has no possible moves
 * Tests both black and white pawns
 */
static void pawn_test1()
{
    board_t curr = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY,  WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  WPAWN4, EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  BPAWN4, EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, EMPTY,  BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1}
        };

    move_list_t w_expected;
    board_t* w_expected_moves[0];
    w_expected.moves = w_expected_moves;
    w_expected.num_moves = 0;

    // generate WPAWN4 moves
    move_list_t *w_actual = generate_pawn_moves(&curr, WPAWN4);

    move_list_t b_expected;
    board_t* b_expected_moves[0];
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 0;

    // generate BPAWN4 moves
    move_list_t *b_actual = generate_pawn_moves(&curr, BPAWN4);

    // CMP boards
    test_result_t w_result = board_list_equals(&w_expected, w_actual);
    test_result_t b_result = board_list_equals(&b_expected, b_actual);

    // display results
    print_test_result(w_result, __func__);
    print_test_result(b_result, __func__);

    // free generated boards
    destroy_move_list(w_actual);
    destroy_move_list(b_actual);
}

/* TEST 2:
 * Pawn can move diagonally to take another piece
 * Test both black and white pawns
 */
static void pawn_test2()
{
    board_t curr = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY,  WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    WPAWN3, EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  BPAWN4, EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, EMPTY,  BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1}
        };

    // move WPAWN3 from (3, 3) to (3, 4)
    board_t* w_expected1 = copy_board(&curr);
    move_piece(w_expected1, WPAWN3, 3, 4);

    // move WPAWN3 from (3, 3) to (4, 4) and take BPAWN4
    board_t* w_expected2 = copy_board(&curr);
    move_piece(w_expected2, WPAWN3, 4, 4);

    // generate WPAWN3 moves
    move_list_t *w_actual = generate_pawn_moves(&curr, WPAWN3);

    move_list_t w_expected;
    board_t* w_expected_moves[2] = {w_expected1, w_expected2};
    w_expected.moves = w_expected_moves;
    w_expected.num_moves = 2;

    // move BPAWN4 from (4, 4) to (4, 3)
    board_t* b_expected1 = copy_board(&curr);
    move_piece(b_expected1, BPAWN4, 4, 3);

    // move BPAWN4 from (4, 4) to (3, 3) and take WPAWN3
    board_t* b_expected2 = copy_board(&curr);
    move_piece(b_expected2, BPAWN4, 3, 3);

    move_list_t b_expected;
    board_t* b_expected_moves[2] = {b_expected1, b_expected2};
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 2;

    // generate BPAWN4 moves
    move_list_t *b_actual = generate_pawn_moves(&curr, BPAWN4);
    
    // CMP boards
    test_result_t w_result = board_list_equals(&w_expected, w_actual);
    test_result_t b_result = board_list_equals(&b_expected, b_actual);

    // display results
    print_test_result(w_result, __func__);
    print_test_result(b_result, __func__);

    // free generated boards
    free(w_expected1);
    free(w_expected2);
    free(b_expected1);
    free(b_expected2);
    destroy_move_list(w_actual);
    destroy_move_list(b_actual);
}

/* TEST 3:
 * Ensure a pawn can only move ahead one space if it has already made its' initial move
 * Tests both black and white pawns
 */
static void pawn_test3()
{
    board_t curr = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY,  WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    BPAWN7},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   EMPTY},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1}
        };

    // move WPAWN0 from (0, 3) to (0, 4)
    board_t* w_expected1 = copy_board(&curr);
    move_piece(w_expected1, WPAWN0, 0, 4);

    move_list_t w_expected;
    board_t* w_expected_moves[1] = {w_expected1};
    w_expected.moves = w_expected_moves;
    w_expected.num_moves = 1;

    // generate WPAWN0 moves
    move_list_t *w_actual = generate_pawn_moves(&curr, WPAWN0);

    // move BPAWN7 from (7, 4) to (7, 3)
    board_t* b_expected1 = copy_board(&curr);
    move_piece(b_expected1, BPAWN7, 7, 3);

    move_list_t b_expected;
    board_t* b_expected_moves[1] = {b_expected1};
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 1;

    // generate BPAWN7 moves
    move_list_t *b_actual = generate_pawn_moves(&curr, BPAWN7);
    
    // CMP boards
    test_result_t w_result = board_list_equals(&w_expected, w_actual);
    test_result_t b_result = board_list_equals(&b_expected, b_actual);

    // display results
    print_test_result(w_result, __func__);
    print_test_result(b_result, __func__);

    // free generated boards
    free(w_expected1);
    free(b_expected1);
    destroy_move_list(w_actual);
    destroy_move_list(b_actual);
}

/* TEST 4:
 * No friendly fire! Ensure a pawn can't take a piece the same colour as itself
 * Tests both black and white pawns
 */
static void pawn_test4()
{
    board_t curr = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY,  EMPTY,    WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    BPAWN6,   EMPTY},
            {EMPTY,  WPAWN1,   EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    BPAWN7},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   EMPTY,    EMPTY},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1}
        };

    // move WPAWN0 from (0, 3) to (0, 4)
    board_t* w_expected1 = copy_board(&curr);
    move_piece(w_expected1, WPAWN0, 0, 4);
    
    move_list_t w_expected;
    board_t* w_expected_moves[1] = {w_expected1};
    w_expected.moves = w_expected_moves;
    w_expected.num_moves = 1;

    // generate WPAWN0 moves
    move_list_t *w_actual = generate_pawn_moves(&curr, WPAWN0);

    // move BPAWN7 from (7, 4) to (7, 3)
    board_t* b_expected1 = copy_board(&curr);
    move_piece(b_expected1, BPAWN7, 7, 3);

    move_list_t b_expected;
    board_t* b_expected_moves[1] = {b_expected1};
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 1;

    // generate BPAWN7 moves
    move_list_t *b_actual = generate_pawn_moves(&curr, BPAWN7);
    
    // CMP boards
    test_result_t w_result = board_list_equals(&w_expected, w_actual);
    test_result_t b_result = board_list_equals(&b_expected, b_actual);

    // display results
    print_test_result(w_result, __func__);
    print_test_result(b_result, __func__);

    // free generated boards
    free(w_expected1);
    free(b_expected1);
    destroy_move_list(w_actual);
    destroy_move_list(b_actual);
}

/*
 * Run the 5 pawn tests
 */
void test_pawn_generator()
{
    // call all 5 tests
    pawn_test0();
    pawn_test1();
    pawn_test2();
    pawn_test3();
    pawn_test4();
}