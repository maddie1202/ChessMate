#include "../../include/game.h"
#include "../../include/possible-move-generators.h"
#include "../include/test.h"
#include <stdlib.h>
#include <stdio.h>

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
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY,  WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  WPAWN4, EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  BPAWN4, EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, EMPTY,  BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1}
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
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY,  WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    WPAWN3, EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  BPAWN4, EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, EMPTY,  BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1}
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
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY,  WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    BPAWN7},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   EMPTY},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1}
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
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY,  EMPTY,    WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    BPAWN6,   EMPTY},
            {EMPTY,  WPAWN1,   EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    BPAWN7},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   EMPTY,    EMPTY},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1}
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

/* TEST 5:
 * Test pawn upgrade to each type of piece (bishop, knight, rook, queen)
 * Tests both black and white pawns
 */
static void pawn_test5()
{
    board_t curr = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WKING,  WQUEEN0,   EMPTY,    EMPTY,  BPAWN4, BPAWN5,  BPAWN6,   BPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY,  EMPTY,    BQUEEN0,  BKING},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  BBISHOP1, BKNIGHT1, BROOK1}
        };

    board_t wp0_expected1 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WKING,  WQUEEN0,  EMPTY,    EMPTY,  BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  WPAWN1,   WPAWN2,   WPAWN3, EMPTY,  EMPTY,    BQUEEN0,  BKING},
            {WROOK2, EMPTY,    EMPTY,    EMPTY,  EMPTY,  BBISHOP1, BKNIGHT1, BROOK1}
        };

    board_t wp0_expected2 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WKING,  WQUEEN0,  EMPTY,    EMPTY,  BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  WPAWN1,   WPAWN2,   WPAWN3, EMPTY,  EMPTY,    BQUEEN0,  BKING},
            {WBISHOP2, EMPTY,    EMPTY,    EMPTY,  EMPTY,  BBISHOP1, BKNIGHT1, BROOK1}
        };

    board_t wp0_expected3 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WKING,  WQUEEN0,  EMPTY,    EMPTY,  BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  WPAWN1,   WPAWN2,   WPAWN3, EMPTY,  EMPTY,    BQUEEN0,  BKING},
            {WKNIGHT2, EMPTY,    EMPTY,    EMPTY,  EMPTY,  BBISHOP1, BKNIGHT1, BROOK1}
        };

    board_t wp0_expected4 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WKING,  WQUEEN0,  EMPTY,    EMPTY,  BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  WPAWN1,   WPAWN2,   WPAWN3, EMPTY,  EMPTY,    BQUEEN0,  BKING},
            {WQUEEN1, EMPTY,    EMPTY,    EMPTY,  EMPTY,  BBISHOP1, BKNIGHT1, BROOK1}
        };

    move_list_t wp0_expected;
    board_t* wp0_expected_moves[4] = {&wp0_expected1, &wp0_expected2, &wp0_expected3, &wp0_expected4};
    wp0_expected.moves = wp0_expected_moves;
    wp0_expected.num_moves = 4;
    
    // generate WPAWN0 moves (upgrade to rook)
    move_list_t *wp0_actual = generate_pawn_moves(&curr, WPAWN0);

    board_t bp4_expected1 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,  BQUEEN5, EMPTY,    EMPTY,    EMPTY},
            {WKING,  WQUEEN0,  EMPTY,    EMPTY,  EMPTY,   BPAWN5,   BPAWN6,   BPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY,   EMPTY,    BQUEEN0,  BKING},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   BBISHOP1, BKNIGHT1, BROOK1}
        };

    board_t bp4_expected2 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,  BKNIGHT6, EMPTY,    EMPTY,    EMPTY},
            {WKING,  WQUEEN0,  EMPTY,    EMPTY,  EMPTY,   BPAWN5,   BPAWN6,   BPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY,   EMPTY,    BQUEEN0,  BKING},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   BBISHOP1, BKNIGHT1, BROOK1}
        };

    board_t bp4_expected3 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,  BBISHOP6, EMPTY,    EMPTY,    EMPTY},
            {WKING,  WQUEEN0,  EMPTY,    EMPTY,  EMPTY,   BPAWN5,   BPAWN6,   BPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY,   EMPTY,    BQUEEN0,  BKING},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   BBISHOP1, BKNIGHT1, BROOK1}
        };

    board_t bp4_expected4 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,  BROOK6, EMPTY,    EMPTY,    EMPTY},
            {WKING,  WQUEEN0,  EMPTY,    EMPTY,  EMPTY,   BPAWN5,   BPAWN6,   BPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY,   EMPTY,    BQUEEN0,  BKING},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,   BBISHOP1, BKNIGHT1, BROOK1}
        };

    move_list_t bp4_expected;
    board_t* bp4_expected_moves[4] = {&bp4_expected1, &bp4_expected2, &bp4_expected3, &bp4_expected4};
    bp4_expected.moves = bp4_expected_moves;
    bp4_expected.num_moves = 4;

    // generate BPAWN4 moves (upgrade to queen)
    move_list_t *bp4_actual = generate_pawn_moves(&curr, BPAWN4);

    // CMP boards
    test_result_t wp0_result = board_list_equals(&wp0_expected, wp0_actual);\
    test_result_t bp4_result = board_list_equals(&bp4_expected, bp4_actual);

    // display results
    print_test_result(wp0_result, __func__);
    print_test_result(bp4_result, __func__);

    // free generated boards
    destroy_move_list(wp0_actual);
    destroy_move_list(bp4_actual);
}

/* TEST 6:
 * Pawn generator shouldn't run when not given a pawn.
 */
static void pawn_test6()
{
    board_t *curr = init_board();

    // try to generate pawn moves for a rook
    move_list_t *w_actual = generate_pawn_moves(curr, WROOK1);

    // try to generate pawn moves for a king
    move_list_t *b_actual = generate_pawn_moves(curr, BKING);

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

    // free generated board
    free(curr);
}

/*
 * Run the pawn tests
 */
void test_pawn_generator()
{
    pawn_test0();
    pawn_test1();
    pawn_test2();
    pawn_test3();
    pawn_test4();
    pawn_test5();
    pawn_test6();
}