#include "../../include/game.h"
#include "../../include/possible-move-generators.h"
#include "../include/test.h"
#include <stdlib.h>
#include <stdio.h>

/* 
 * This file contains 4 tests for the king move generator.
 * TEST 0: No possible moves/can't take pieces of the same colour as themselves (initial board)
 * TEST 1: Move one forward, one backwards, one left, one right, or one diagonally
 * TEST 2: Take an enemy piece/cannot move into check
 * TEST 3: Move out of check
 * TEST 4: no possible moves (checkmate)
 */

/* TEST 0: 
 * Test the king straight from initial setup for both black and white 
 * King has no possible moves
 */
static void king_test0()
{
    board_t* curr = init_board();

    move_list_t w_expected;
    board_t* w_expected_moves[0];
    w_expected.moves = w_expected_moves;
    w_expected.num_moves = 0;

    // generate WKING moves
    move_list_t *w_actual = generate_king_moves(curr, WKING);

    move_list_t b_expected;
    board_t* b_expected_moves[0];
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 0;

    // generate BKING moves
    move_list_t *b_actual = generate_king_moves(curr, BKING);

    // CMP boards
    test_result_t w_result = board_list_equals(&w_expected, w_actual);
    test_result_t b_result = board_list_equals(&b_expected, b_actual);

    // display results
    print_test_result(w_result, __func__);
    print_test_result(b_result, __func__);

    // free generated boards
    free(curr);
    destroy_move_list(w_actual);
    destroy_move_list(b_actual);
}

/* TEST 1: 
 * Test the king's movements in each direction
 * No movement will put the king in check
 * Tests both black and white kings
 */
static void king_test1()
{
    board_t curr = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,  EMPTY,  EMPTY,    WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY,  WKING,  EMPTY,    EMPTY,    WPAWN7},       
            {EMPTY,  EMPTY,    WQUEEN0,   EMPTY,  EMPTY,  EMPTY,    WPAWN6,   WBISHOP1},
            {EMPTY,  EMPTY,    EMPTY,    WPAWN3, WPAWN4, WPAWN5,   EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    BPAWN3, BPAWN4, BPAWN5,   EMPTY,    EMPTY},
            {EMPTY,  BQUEEN0,   EMPTY,    EMPTY,  EMPTY,  EMPTY,    BPAWN6,   BBISHOP1},
            {BPAWN0, BPAWN1,   BPAWN2,   EMPTY,  BKING,  EMPTY,    EMPTY,    BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, EMPTY,  EMPTY,  EMPTY,    BKNIGHT1, BROOK1}
        };

    // move WKING left from (4, 1) to (3, 1)
    board_t* w_expected1 = copy_board(&curr);
    move_piece(w_expected1, WKING, 3, 1);

    // move WKING diagonally from (4, 1) to (3, 2)
    board_t* w_expected2 = copy_board(&curr);
    move_piece(w_expected2, WKING, 3, 2);
    
    // move WKING forward from (4, 1) to (4, 2)
    board_t* w_expected3 = copy_board(&curr);
    move_piece(w_expected3, WKING, 4, 2);

    // move WKING diagonally from (4, 1) to (5, 2)
    board_t* w_expected4 = copy_board(&curr);
    move_piece(w_expected4, WKING, 5, 2);

    // move WKING right from (4, 1) to (5, 1)
    board_t* w_expected5 = copy_board(&curr);
    move_piece(w_expected5, WKING, 5, 1);

    // move WKING diagonally from (4, 1) to (5, 0)
    board_t* w_expected6 = copy_board(&curr);
    move_piece(w_expected6, WKING, 5, 0);

    // move WKING backward from (4, 1) to (4, 0)
    board_t* w_expected7 = copy_board(&curr);
    move_piece(w_expected7, WKING, 4, 0);

    // move WKING diagonally from (4, 1) to (3, 0)
    board_t* w_expected8 = copy_board(&curr);
    move_piece(w_expected8, WKING, 3, 0);

    move_list_t w_expected;
    board_t* w_expected_moves[8] = {w_expected1, w_expected2, w_expected3, w_expected4, 
                                    w_expected5, w_expected6, w_expected7, w_expected8};
    w_expected.moves = w_expected_moves;
    w_expected.num_moves = 8;

    // generate WKING moves
    move_list_t *w_actual = generate_king_moves(&curr, WKING);

    // move BKING left from (4, 6) to (3, 6)
    board_t* b_expected1 = copy_board(&curr);
    move_piece(b_expected1, BKING, 3, 6);

    // move BKING diagonally from (4, 6) to (3, 5)
    board_t* b_expected2 = copy_board(&curr);
    move_piece(b_expected2, BKING, 3, 5);
    
    // move BKING forward from (4, 6) to (4, 5)
    board_t* b_expected3 = copy_board(&curr);
    move_piece(b_expected3, BKING, 4, 5);

    // move BKING diagonally from (4, 6) to (5, 5)
    board_t* b_expected4 = copy_board(&curr);
    move_piece(b_expected4, BKING, 5, 5);

    // move BKING right from (4, 6) to (5, 6)
    board_t* b_expected5 = copy_board(&curr);
    move_piece(b_expected5, BKING, 5, 6);

    // move BKING diagonally from (4, 6) to (5, 7)
    board_t* b_expected6 = copy_board(&curr);
    move_piece(b_expected6, BKING, 5, 7);

    // move BKING backward from (4, 6) to (4, 7)
    board_t* b_expected7 = copy_board(&curr);
    move_piece(b_expected7, BKING, 4, 7);

    // move BKING diagonally from (4, 6) to (3, 7)
    board_t* b_expected8 = copy_board(&curr);
    move_piece(b_expected8, BKING, 3, 7);

    move_list_t b_expected;
    board_t* b_expected_moves[8] = {b_expected1, b_expected2, b_expected3, b_expected4, 
                                    b_expected5, b_expected6, b_expected7, b_expected8};
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 8;

    // generate BKING moves
    move_list_t *b_actual = generate_king_moves(&curr, BKING);

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
    free(w_expected7);
    free(w_expected8);
    free(b_expected1);
    free(b_expected2);
    free(b_expected3);
    free(b_expected4);
    free(b_expected5);
    free(b_expected6);
    free(b_expected7);
    free(b_expected8);
    destroy_move_list(w_actual);
    destroy_move_list(b_actual);
}


/* TEST 2: 
 * Take an enemy piece
 * Tests both black and white kings
 */
static void king_test2()
{
    board_t curr = {
            {WROOK0, EMPTY,  WBISHOP0, WQUEEN0,   EMPTY,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1, WPAWN2,   WKNIGHT0, EMPTY,  WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,  WKING,    WPAWN3,   EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,  BPAWN2,   EMPTY,    EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,  EMPTY,    EMPTY,    WPAWN4, EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,  EMPTY,    BPAWN3,   BKING,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1, EMPTY,    BKNIGHT0, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, EMPTY,  BBISHOP0, BQUEEN0,   EMPTY,  BBISHOP1, BKNIGHT1, BROOK1}
        };

    // NOTE: WKING can't move left to (1, 2) IRL, since it would move into check
    board_t* w_expected1 = copy_board(&curr);
    move_piece(w_expected1, WKING, 1, 2);

    // move WKING diagonally from (2, 2) to (1, 3)
    board_t* w_expected2 = copy_board(&curr);
    move_piece(w_expected2, WKING, 1, 3);
    
    // move WKING forward from (2, 2) to (2, 3) and take BPAWN2
    board_t* w_expected3 = copy_board(&curr);
    move_piece(w_expected3, WKING, 2, 3);

    // move WKING diagonally from (2, 2) to (3, 3)
    board_t* w_expected4 = copy_board(&curr);
    move_piece(w_expected4, WKING, 3, 3);

    move_list_t w_expected;
    board_t* w_expected_moves[4] = {w_expected1, w_expected2, w_expected3, w_expected4};
    w_expected.moves = w_expected_moves;
    w_expected.num_moves = 4;

    // generate WKING moves
    move_list_t *w_actual = generate_king_moves(&curr, WKING);

    // NOTE: BKING can't move right to (5, 5) IRL, since it would move into check
    board_t* b_expected1 = copy_board(&curr);
    move_piece(b_expected1, BKING, 5, 5);

    // move BKING diagonally from (4, 5) to (5, 4)
    board_t* b_expected2 = copy_board(&curr);
    move_piece(b_expected2, BKING, 5, 4);

    // move BKING forward from (4, 5) to (4, 4) and take WPAWN4
    board_t* b_expected3 = copy_board(&curr);
    move_piece(b_expected3, BKING, 4, 4);
    
    // move BKING diagonally from (4, 5) to (3, 4)
    board_t* b_expected4 = copy_board(&curr);
    move_piece(b_expected4, BKING, 3, 4);

    move_list_t b_expected;
    board_t* b_expected_moves[4] = {b_expected1, b_expected2, b_expected3, b_expected4};
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 4;

    // generate BKING moves
    move_list_t *b_actual = generate_king_moves(&curr, BKING);

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
    free(b_expected1);
    free(b_expected2);
    free(b_expected3);
    free(b_expected4);
    destroy_move_list(w_actual);
    destroy_move_list(b_actual);
}

/* TEST 3: 
 * Tries to run the king generator on other pieces
 * Tests both black and white kings
 */
static void king_test3()
{
    board_t *curr = init_board();

    // generate WKING moves for a rook
    move_list_t *w_actual = generate_king_moves(curr, WROOK0);

    // generate BKING moves for a knight
    move_list_t *b_actual = generate_king_moves(curr, BKNIGHT1);

    test_result_t w_result;
    if (w_actual != NULL) {
        w_result = test_result(0, "Should be NULL");
        free(w_actual);
    } else w_result = test_result(1, "");

    test_result_t b_result;
    if (b_actual != NULL) {
        b_result = test_result(0, "Should be NULL");
        free(b_actual);
    } else b_result = test_result(1, "");

    // display results
    print_test_result(w_result, __func__);
    print_test_result(b_result, __func__);

    // free generated boards
    free(curr);
}

/*
 * Run the king tests
 */
void test_king_generator()
{
    king_test0();
    king_test1();
    king_test2();
    king_test3();
}