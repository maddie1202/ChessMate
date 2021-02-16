#include "../../include/game.h"
#include "../../include/possible-move-generators.h"
#include "../include/test.h"
#include <stdlib.h>
#include <stdio.h>

/* TEST 0:
 * Tests legal kingside castling for both black and white
 */
static void castling_test0()
{
    board_t curr = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,   EMPTY,  EMPTY,  WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WBISHOP1, WPAWN5, WPAWN6, WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,    EMPTY,  EMPTY,  WKNIGHT1},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  WPAWN4,   EMPTY,  EMPTY,  EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  BPAWN4,   EMPTY,  EMPTY,  EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,    EMPTY,  EMPTY,  BKNIGHT1},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BBISHOP1, BPAWN5, BPAWN6, BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,   EMPTY,  EMPTY,  BROOK1}
        };

    game_t game;
    game.board = &curr;
    game.bking_has_moved = false;
    game.brook0_has_moved = false;
    game.brook1_has_moved = false;
    game.wking_has_moved = false;
    game.wrook0_has_moved = false;
    game.wrook1_has_moved = false;
    
    board_t w_expected1 = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,   EMPTY,  EMPTY,  WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WBISHOP1, WPAWN5, WPAWN6, WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,    EMPTY,  EMPTY,  WKNIGHT1},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  WPAWN4,   EMPTY,  EMPTY,  EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  BPAWN4,   EMPTY,  EMPTY,  EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,    EMPTY,  EMPTY,  BKNIGHT1},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BBISHOP1, BPAWN5, BPAWN6, BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, EMPTY,   BROOK1, BKING,  EMPTY}
        };

    // generate white castling moves
    move_list_t *w_actual = generate_castling_moves(&game, WKING, WROOK1);

    move_list_t w_expected;
    board_t* w_expected_moves[1] = {&w_expected1};
    w_expected.moves = w_expected_moves;
    w_expected.num_moves = 1;

    board_t b_expected1 = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,   EMPTY,  EMPTY,  WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WBISHOP1, WPAWN5, WPAWN6, WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,    EMPTY,  EMPTY,  WKNIGHT1},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  WPAWN4,   EMPTY,  EMPTY,  EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  BPAWN4,   EMPTY,  EMPTY,  EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,    EMPTY,  EMPTY,  BKNIGHT1},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BBISHOP1, BPAWN5, BPAWN6, BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, EMPTY,   BROOK1, BKING,  EMPTY}
        };

    // generate black castling moves
    move_list_t *b_actual = generate_castling_moves(&game, BKING, BROOK1);

    move_list_t b_expected;
    board_t* b_expected_moves[1] = {&b_expected1};
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 1;

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

/* TEST 1:
 * Tests legal queenside castling for both black and white
 */
static void castling_test1()
{
    board_t curr = {
            {WROOK0,   EMPTY,  EMPTY,  EMPTY,    WKING,   WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0,   WPAWN1, WPAWN2, WBISHOP0, WQUEEN0, WPAWN5,   WPAWN6,   WPAWN7},       
            {WKNIGHT0, EMPTY,  EMPTY,  WPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    BPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {BKNIGHT0, EMPTY,  EMPTY,  BPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {BPAWN0,   BPAWN1, BPAWN2, BBISHOP0, BQUEEN0, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0,   EMPTY,  EMPTY,  EMPTY,    BKING,   BBISHOP1, BKNIGHT1, BROOK1}
        };

    game_t game;
    game.board = &curr;
    game.bking_has_moved = false;
    game.brook0_has_moved = false;
    game.brook1_has_moved = false;
    game.wking_has_moved = false;
    game.wrook0_has_moved = false;
    game.wrook1_has_moved = false;

    board_t w_expected1 = {
            {EMPTY,    EMPTY,  WKING,  WROOK0,   EMPTY,   WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0,   WPAWN1, WPAWN2, WBISHOP0, WQUEEN0, WPAWN5,   WPAWN6,   WPAWN7},       
            {WKNIGHT0, EMPTY,  EMPTY,  WPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    BPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {BKNIGHT0, EMPTY,  EMPTY,  BPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {BPAWN0,   BPAWN1, BPAWN2, BBISHOP0, BQUEEN0, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0,   EMPTY,  EMPTY,  EMPTY,    BKING,   BBISHOP1, BKNIGHT1, BROOK1}
        };

    // generate white castling moves
    move_list_t *w_actual = generate_castling_moves(&game, WKING, WROOK0);

    move_list_t w_expected;
    board_t* w_expected_moves[1] = {&w_expected1};
    w_expected.moves = w_expected_moves;
    w_expected.num_moves = 1;

    board_t b_expected1 = {
            {WROOK0,   EMPTY,  EMPTY,  EMPTY,    WKING,   WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0,   WPAWN1, WPAWN2, WBISHOP0, WQUEEN0, WPAWN5,   WPAWN6,   WPAWN7},       
            {WKNIGHT0, EMPTY,  EMPTY,  WPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    BPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {BKNIGHT0, EMPTY,  EMPTY,  BPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {BPAWN0,   BPAWN1, BPAWN2, BBISHOP0, BQUEEN0, BPAWN5,   BPAWN6,   BPAWN7},
            {EMPTY,    EMPTY,  BKING,  BROOK0,   EMPTY,   BBISHOP1, BKNIGHT1, BROOK1}
        };

    // generate black castling moves
    move_list_t *b_actual = generate_castling_moves(&game, BKING, BROOK0);

    move_list_t b_expected;
    board_t* b_expected_moves[1] = {&b_expected1};
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 1;

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
 * Tries to perform kingside castling when the rook has previously 
 * moved from its intial position
 * Tests both black and white
 */
static void castling_test2()
{
    board_t curr = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,   EMPTY,  EMPTY,  WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WBISHOP1, WPAWN5, WPAWN6, WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,    EMPTY,  EMPTY,  WKNIGHT1},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  WPAWN4,   EMPTY,  EMPTY,  EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  BPAWN4,   EMPTY,  EMPTY,  EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,    EMPTY,  EMPTY,  BKNIGHT1},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BBISHOP1, BPAWN5, BPAWN6, BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,   EMPTY,  EMPTY,  BROOK1}
        };

    game_t game;
    game.board = &curr;
    game.bking_has_moved = false;
    game.brook0_has_moved = false;
    game.brook1_has_moved = true;
    game.wking_has_moved = false;
    game.wrook0_has_moved = false;
    game.wrook1_has_moved = true;

    // generate white castling moves
    move_list_t *w_actual = generate_castling_moves(&game, WKING, WROOK1);

    move_list_t w_expected;
    board_t* w_expected_moves[0] = {};
    w_expected.moves = w_expected_moves;
    w_expected.num_moves = 0;

    // generate black castling moves
    move_list_t *b_actual = generate_castling_moves(&game, BKING, BROOK1);

    move_list_t b_expected;
    board_t* b_expected_moves[0] = {};
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 0;

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

/* TEST 3:
 * Tries to perform kingside castling when the king has previously 
 * moved from its initial position
 * Tests both black and white
 */
static void castling_test3()
{
    board_t curr = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,   EMPTY,  EMPTY,  WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WBISHOP1, WPAWN5, WPAWN6, WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,    EMPTY,  EMPTY,  WKNIGHT1},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  WPAWN4,   EMPTY,  EMPTY,  EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  BPAWN4,   EMPTY,  EMPTY,  EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,    EMPTY,  EMPTY,  BKNIGHT1},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BBISHOP1, BPAWN5, BPAWN6, BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,   EMPTY,  EMPTY,  BROOK1}
        };

    game_t game;
    game.board = &curr;
    game.bking_has_moved = true;
    game.brook0_has_moved = false;
    game.brook1_has_moved = false;
    game.wking_has_moved = true;
    game.wrook0_has_moved = false;
    game.wrook1_has_moved = false;

    // generate white castling moves
    move_list_t *w_actual = generate_castling_moves(&game, WKING, WROOK1);

    move_list_t w_expected;
    board_t* w_expected_moves[0] = {};
    w_expected.moves = w_expected_moves;
    w_expected.num_moves = 0;

    // generate black castling moves
    move_list_t *b_actual = generate_castling_moves(&game, BKING, BROOK1);

    move_list_t b_expected;
    board_t* b_expected_moves[0] = {};
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 0;

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

/* TEST 4:
 * Tries to perform queenside castling when the rook has previously 
 * moved from its initial position
 * Tests both black and white
 */
static void castling_test4()
{
    board_t curr = {
            {WROOK0,   EMPTY,  EMPTY,  EMPTY,    WKING,   WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0,   WPAWN1, WPAWN2, WBISHOP0, WQUEEN0, WPAWN5,   WPAWN6,   WPAWN7},       
            {WKNIGHT0, EMPTY,  EMPTY,  WPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    BPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {BKNIGHT0, EMPTY,  EMPTY,  BPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {BPAWN0,   BPAWN1, BPAWN2, BBISHOP0, BQUEEN0, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0,   EMPTY,  EMPTY,  EMPTY,    BKING,   BBISHOP1, BKNIGHT1, BROOK1}
        };

    game_t game;
    game.board = &curr;
    game.bking_has_moved = false;
    game.brook0_has_moved = true;
    game.brook1_has_moved = false;
    game.wking_has_moved = false;
    game.wrook0_has_moved = true;
    game.wrook1_has_moved = false;

    // generate white castling moves
    move_list_t *w_actual = generate_castling_moves(&game, WKING, WROOK0);

    move_list_t w_expected;
    board_t* w_expected_moves[0] = {};
    w_expected.moves = w_expected_moves;
    w_expected.num_moves = 0;

    // generate black castling moves
    move_list_t *b_actual = generate_castling_moves(&game, BKING, BROOK0);

    move_list_t b_expected;
    board_t* b_expected_moves[0] = {};
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 0;

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

/* TEST 5:
 * Tries to perform queenside castling when the king has previously
 * moved from its initial position
 * Tests both black and white
 */
static void castling_test5()
{
    board_t curr = {
            {WROOK0,   EMPTY,  EMPTY,  EMPTY,    WKING,   WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0,   WPAWN1, WPAWN2, WBISHOP0, WQUEEN0, WPAWN5,   WPAWN6,   WPAWN7},       
            {WKNIGHT0, EMPTY,  EMPTY,  WPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    BPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {BKNIGHT0, EMPTY,  EMPTY,  BPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {BPAWN0,   BPAWN1, BPAWN2, BBISHOP0, BQUEEN0, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0,   EMPTY,  EMPTY,  EMPTY,    BKING,   BBISHOP1, BKNIGHT1, BROOK1}
        };

    game_t game;
    game.board = &curr;
    game.bking_has_moved = true;
    game.brook0_has_moved = false;
    game.brook1_has_moved = false;
    game.wking_has_moved = true;
    game.wrook0_has_moved = false;
    game.wrook1_has_moved = false;

    // generate white castling moves
    move_list_t *w_actual = generate_castling_moves(&game, WKING, WROOK0);

    move_list_t w_expected;
    board_t* w_expected_moves[0] = {};
    w_expected.moves = w_expected_moves;
    w_expected.num_moves = 0;

    // generate black castling moves
    move_list_t *b_actual = generate_castling_moves(&game, BKING, BROOK0);

    move_list_t b_expected;
    board_t* b_expected_moves[0] = {};
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 0;

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

/* TEST 6:
 * Make sure we can't perform castling when there is another piece 
 * in the way
 * Tests both black and white
 */
static void castling_test6()
{
    board_t curr = {
            {WROOK0, WKNIGHT0, EMPTY,  EMPTY,    WKING,   WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2, WBISHOP0, WQUEEN0, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,  WPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,  EMPTY,    WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,  EMPTY,    BPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,  BPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2, BBISHOP0, BQUEEN0, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, EMPTY,  EMPTY,    BKING,   BBISHOP1, BKNIGHT1, BROOK1}
        };

    game_t game;
    game.board = &curr;
    game.bking_has_moved = false;
    game.brook0_has_moved = false;
    game.brook1_has_moved = false;
    game.wking_has_moved = false;
    game.wrook0_has_moved = false;
    game.wrook1_has_moved = false;

    // generate white kingside castling moves
    move_list_t *wk_actual = generate_castling_moves(&game, WKING, WROOK1);

    move_list_t wk_expected;
    board_t* wk_expected_moves[0] = {};
    wk_expected.moves = wk_expected_moves;
    wk_expected.num_moves = 0;

    // generate white queenside castling moves
    move_list_t *wq_actual = generate_castling_moves(&game, WKING, WROOK0);

    move_list_t wq_expected;
    board_t* wq_expected_moves[0] = {};
    wq_expected.moves = wq_expected_moves;
    wq_expected.num_moves = 0;

    // generate black kingside castling moves
    move_list_t *bk_actual = generate_castling_moves(&game, BKING, BROOK1);

    move_list_t bk_expected;
    board_t* bk_expected_moves[0] = {};
    bk_expected.moves = bk_expected_moves;
    bk_expected.num_moves = 0;

    // generate black kingside castling moves
    move_list_t *bq_actual = generate_castling_moves(&game, BKING, BROOK0);

    move_list_t bq_expected;
    board_t* bq_expected_moves[0] = {};
    bq_expected.moves = bq_expected_moves;
    bq_expected.num_moves = 0;

    // CMP boards
    test_result_t wk_result = board_list_equals(&wk_expected, wk_actual);
    test_result_t wq_result = board_list_equals(&wq_expected, wq_actual);
    test_result_t bk_result = board_list_equals(&bk_expected, bk_actual);
    test_result_t bq_result = board_list_equals(&bq_expected, bq_actual);

    // display results
    print_test_result(wk_result, __func__);
    print_test_result(wq_result, __func__);
    print_test_result(bk_result, __func__);
    print_test_result(bq_result, __func__);

    // free generated boards
    destroy_move_list(wk_actual);
    destroy_move_list(wq_actual);
    destroy_move_list(bk_actual);
    destroy_move_list(bq_actual);
}

/* TEST 7:
 * Tries to perform castling when the pieces aren't in their 
 * original locations
 * Tests both black and white
 */
static void castling_test7()
{
    board_t curr = {
            {EMPTY,  WKNIGHT0, WBISHOP0, WQUEEN0, EMPTY,  WBISHOP1, WKNIGHT1, EMPTY},   
            {WROOK0, EMPTY,    EMPTY,    EMPTY,   WKING,  EMPTY,    EMPTY,    WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3,  WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},    
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,   EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,   EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3,  BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, EMPTY,    EMPTY,    EMPTY,   BKING,  EMPTY,    EMPTY,    BROOK1},
            {EMPTY,  BKNIGHT0, BBISHOP0, BQUEEN0, EMPTY,  BBISHOP1, BKNIGHT1, EMPTY}
        };

    game_t game;
    game.board = &curr;
    game.bking_has_moved = true;
    game.brook0_has_moved = true;
    game.brook1_has_moved = true;
    game.wking_has_moved = true;
    game.wrook0_has_moved = true;
    game.wrook1_has_moved = true;

    // generate white kingside castling moves
    move_list_t *wk_actual = generate_castling_moves(&game, WKING, WROOK1);

    move_list_t wk_expected;
    board_t* wk_expected_moves[0] = {};
    wk_expected.moves = wk_expected_moves;
    wk_expected.num_moves = 0;

    // generate white queenside castling moves
    move_list_t *wq_actual = generate_castling_moves(&game, WKING, WROOK0);

    move_list_t wq_expected;
    board_t* wq_expected_moves[0] = {};
    wq_expected.moves = wq_expected_moves;
    wq_expected.num_moves = 0;

    // generate black kingside castling moves
    move_list_t *bk_actual = generate_castling_moves(&game, BKING, BROOK1);

    move_list_t bk_expected;
    board_t* bk_expected_moves[0] = {};
    bk_expected.moves = bk_expected_moves;
    bk_expected.num_moves = 0;

    // generate black kingside castling moves
    move_list_t *bq_actual = generate_castling_moves(&game, BKING, BROOK0);

    move_list_t bq_expected;
    board_t* bq_expected_moves[0] = {};
    bq_expected.moves = bq_expected_moves;
    bq_expected.num_moves = 0;

    // CMP boards
    test_result_t wk_result = board_list_equals(&wk_expected, wk_actual);
    test_result_t wq_result = board_list_equals(&wq_expected, wq_actual);
    test_result_t bk_result = board_list_equals(&bk_expected, bk_actual);
    test_result_t bq_result = board_list_equals(&bq_expected, bq_actual);

    // display results
    print_test_result(wk_result, __func__);
    print_test_result(wq_result, __func__);
    print_test_result(bk_result, __func__);
    print_test_result(bq_result, __func__);

    // free generated boards
    destroy_move_list(wk_actual);
    destroy_move_list(wq_actual);
    destroy_move_list(bk_actual);
    destroy_move_list(bq_actual);
}

void test_castling_generator()
{
    // NOTE: need to test in check tester that we can't perform castling to get out of check
    // also need to check that we can't castle into check or castle through check
    castling_test0();
    castling_test1();
    castling_test2();
    castling_test3();
    castling_test4();
    castling_test5();
    castling_test6();
    castling_test7();
}