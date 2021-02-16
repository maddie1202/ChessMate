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
 * TEST 5: Tests pawn upgrades
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
    move_list_t *w_actual = generate_pawn_moves(curr, WPAWN2, EMPTY);

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
    move_list_t *b_actual = generate_pawn_moves(curr, BPAWN2, EMPTY);
    
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
    move_list_t *w_actual = generate_pawn_moves(&curr, WPAWN4, EMPTY);

    move_list_t b_expected;
    board_t* b_expected_moves[0];
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 0;

    // generate BPAWN4 moves
    move_list_t *b_actual = generate_pawn_moves(&curr, BPAWN4, EMPTY);

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
    move_list_t *w_actual = generate_pawn_moves(&curr, WPAWN3, EMPTY);

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
    move_list_t *b_actual = generate_pawn_moves(&curr, BPAWN4, EMPTY);
    
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
    move_list_t *w_actual = generate_pawn_moves(&curr, WPAWN0, EMPTY);

    // move BPAWN7 from (7, 4) to (7, 3)
    board_t* b_expected1 = copy_board(&curr);
    move_piece(b_expected1, BPAWN7, 7, 3);

    move_list_t b_expected;
    board_t* b_expected_moves[1] = {b_expected1};
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 1;

    // generate BPAWN7 moves
    move_list_t *b_actual = generate_pawn_moves(&curr, BPAWN7, EMPTY);
    
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
    move_list_t *w_actual = generate_pawn_moves(&curr, WPAWN0, EMPTY);

    // move BPAWN7 from (7, 4) to (7, 3)
    board_t* b_expected1 = copy_board(&curr);
    move_piece(b_expected1, BPAWN7, 7, 3);

    move_list_t b_expected;
    board_t* b_expected_moves[1] = {b_expected1};
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 1;

    // generate BPAWN7 moves
    move_list_t *b_actual = generate_pawn_moves(&curr, BPAWN7, EMPTY);
    
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

    move_list_t wp0_expected;
    board_t* wp0_expected_moves[1] = {&wp0_expected1};
    wp0_expected.moves = wp0_expected_moves;
    wp0_expected.num_moves = 1;
    
    // generate WPAWN0 moves (upgrade to rook)
    move_list_t *wp0_actual = generate_pawn_moves(&curr, WPAWN0, ROOK);

    board_t wp1_expected1 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WKING,  WQUEEN0,  EMPTY,    EMPTY,  BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    WPAWN2,   WPAWN3, EMPTY,  EMPTY,    BQUEEN0,   BKING},
            {EMPTY,  WKNIGHT3, EMPTY,    EMPTY,  EMPTY,  BBISHOP1, BKNIGHT1, BROOK1}
        };
    
    move_list_t wp1_expected;
    board_t* wp1_expected_moves[1] = {&wp1_expected1};
    wp1_expected.moves = wp1_expected_moves;
    wp1_expected.num_moves = 1;

    // generate WPAWN1 moves (upgrade to knight)
    move_list_t *wp1_actual = generate_pawn_moves(&curr, WPAWN1, KNIGHT);

    board_t wp2_expected1 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WKING,  WQUEEN0,  EMPTY,    EMPTY,  BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, WPAWN1,   EMPTY,    WPAWN3, EMPTY,  EMPTY,    BQUEEN0,  BKING},
            {EMPTY,  EMPTY,    WBISHOP4, EMPTY,  EMPTY,  BBISHOP1, BKNIGHT1, BROOK1}
        };
    
    move_list_t wp2_expected;
    board_t* wp2_expected_moves[1] = {&wp2_expected1};
    wp2_expected.moves = wp2_expected_moves;
    wp2_expected.num_moves = 1;

    // generate WPAWN2 moves (upgrade to bishop)
    move_list_t *wp2_actual = generate_pawn_moves(&curr, WPAWN2, BISHOP);

    board_t wp3_expected1 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,   EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WKING,  WQUEEN0,  EMPTY,    EMPTY,   BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,   EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,   EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,   EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,   EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY,   EMPTY,  EMPTY,    BQUEEN0,  BKING},
            {EMPTY,  EMPTY,    EMPTY,    WQUEEN4, EMPTY,  BBISHOP1, BKNIGHT1, BROOK1}
        };

    move_list_t wp3_expected;
    board_t* wp3_expected_moves[1] = {&wp3_expected1};
    wp3_expected.moves = wp3_expected_moves;
    wp3_expected.num_moves = 1;

    // generate WPAWN3 moves (upgrade to queen)
    move_list_t *wp3_actual = generate_pawn_moves(&curr, WPAWN3, QUEEN);

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

    move_list_t bp4_expected;
    board_t* bp4_expected_moves[1] = {&bp4_expected1};
    bp4_expected.moves = bp4_expected_moves;
    bp4_expected.num_moves = 1;

    // generate BPAWN4 moves (upgrade to queen)
    move_list_t *bp4_actual = generate_pawn_moves(&curr, BPAWN4, QUEEN);

    board_t bp5_expected1 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,  EMPTY,  BBISHOP7, EMPTY,    EMPTY},
            {WKING,  WQUEEN0,  EMPTY,    EMPTY,  BPAWN4, EMPTY,    BPAWN6,   BPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY,  EMPTY,    BQUEEN0,  BKING},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  BBISHOP1, BKNIGHT1, BROOK1}
        };

    move_list_t bp5_expected;
    board_t* bp5_expected_moves[1] = {&bp5_expected1};
    bp5_expected.moves = bp5_expected_moves;
    bp5_expected.num_moves = 1;

    // generate BPAWN5 moves (upgrade to bishop)
    move_list_t *bp5_actual = generate_pawn_moves(&curr, BPAWN5, BISHOP);

    board_t bp6_expected1 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,  EMPTY,  EMPTY,    BKNIGHT8, EMPTY},
            {WKING,  WQUEEN0,  EMPTY,    EMPTY,  BPAWN4, BPAWN5,   EMPTY,    BPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY,  EMPTY,    BQUEEN0,  BKING},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  BBISHOP1, BKNIGHT1, BROOK1}
        };

    move_list_t bp6_expected;
    board_t* bp6_expected_moves[1] = {&bp6_expected1};
    bp6_expected.moves = bp6_expected_moves;
    bp6_expected.num_moves = 1;

    // generate BPAWN4 moves (upgrade to knight)
    move_list_t *bp6_actual = generate_pawn_moves(&curr, BPAWN6, KNIGHT);

    board_t bp7_expected1 = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,  EMPTY,  EMPTY,    EMPTY,    BROOK9},
            {WKING,  WQUEEN0,  EMPTY,    EMPTY,  BPAWN4, BPAWN5,   BPAWN6,   EMPTY},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY,  EMPTY,    BQUEEN0,  BKING},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  BBISHOP1, BKNIGHT1, BROOK1}
        };

    move_list_t bp7_expected;
    board_t* bp7_expected_moves[1] = {&bp7_expected1};
    bp7_expected.moves = bp7_expected_moves;
    bp7_expected.num_moves = 1;

    // generate BPAWN7 moves (upgrade to rook)
    move_list_t *bp7_actual = generate_pawn_moves(&curr, BPAWN7, ROOK);

    // CMP boards
    test_result_t wp0_result = board_list_equals(&wp0_expected, wp0_actual);
    test_result_t wp1_result = board_list_equals(&wp1_expected, wp1_actual);
    test_result_t wp2_result = board_list_equals(&wp2_expected, wp2_actual);
    test_result_t wp3_result = board_list_equals(&wp3_expected, wp3_actual);
    test_result_t bp4_result = board_list_equals(&bp4_expected, bp4_actual);
    test_result_t bp5_result = board_list_equals(&bp5_expected, bp5_actual);
    test_result_t bp6_result = board_list_equals(&bp6_expected, bp6_actual);
    test_result_t bp7_result = board_list_equals(&bp7_expected, bp7_actual);

    // display results
    print_test_result(wp0_result, __func__);
    print_test_result(wp1_result, __func__);
    print_test_result(wp2_result, __func__);
    print_test_result(wp3_result, __func__);
    print_test_result(bp4_result, __func__);
    print_test_result(bp5_result, __func__);
    print_test_result(bp6_result, __func__);
    print_test_result(bp7_result, __func__);

    // free generated boards
    destroy_move_list(wp0_actual);
    destroy_move_list(wp1_actual);
    destroy_move_list(wp2_actual);
    destroy_move_list(wp3_actual);
    destroy_move_list(bp4_actual);
    destroy_move_list(bp5_actual);
    destroy_move_list(bp6_actual);
    destroy_move_list(bp7_actual);
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
}