#include "include/test-possible-move-generators.h"
#include "../include/game.h"
#include "../include/possible-move-generators.h"
#include "include/test.h"
#include <stdlib.h>
#include <stdio.h> 

void test_possible_moves()
{
    test_pawn_generator();
    test_rook_generator();
    test_knight_generator();
    test_bishop_generator();
    test_queen_generator();
    test_king_generator();
    test_castling_generator();
    test_all_moves();
}

void test_all_moves()
{
    board_t curr = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY,  WPAWN1,   WPAWN2,   EMPTY,  EMPTY,  EMPTY,    WPAWN6,   WPAWN7},       
            {WPAWN0, EMPTY,    EMPTY,    WPAWN3, WPAWN4, WPAWN5,   EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, EMPTY,    EMPTY,    BPAWN3, BPAWN4, BPAWN5,   EMPTY,    EMPTY},
            {EMPTY,  BPAWN1,   BPAWN2,   EMPTY,  EMPTY,  EMPTY,    BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN, BKING,  BBISHOP1, BKNIGHT1, BROOK1}
        };

    game_t game;
    game.board = &curr;
    game.bking_has_moved = false;
    game.brook0_has_moved = false;
    game.brook1_has_moved = false;
    game.wking_has_moved = false;
    game.wrook0_has_moved = false;
    game.wrook1_has_moved = false;

    // WPAWN0 (1)
    board_t* wpawn0 = copy_board(&curr);
    move_piece(wpawn0, WPAWN0, 0, 3); // from (0, 2) to (0, 3)
    
    // WPAWN1 (2)
    board_t* wpawn1_1 = copy_board(&curr);
    move_piece(wpawn1_1, WPAWN1, 1, 2); // from (1, 1) to (1, 2)
    board_t* wpawn1_2 = copy_board(&curr);
    move_piece(wpawn1_2, WPAWN1, 1, 3); // from (1, 1) to (1, 3)

    // WPAWN2 (2)
    board_t* wpawn2_1 = copy_board(&curr);
    move_piece(wpawn2_1, WPAWN2, 2, 2); // from (2, 1) to (2, 2)
    board_t* wpawn2_2 = copy_board(&curr);
    move_piece(wpawn2_2, WPAWN2, 2, 3); // from (2, 1) to (2, 3)

    // WPAWN3 (1)
    board_t* wpawn3 = copy_board(&curr);
    move_piece(wpawn3, WPAWN3, 3, 3); // from (3, 2) to (3, 3)

    // WPAWN4 (1)
    board_t* wpawn4 = copy_board(&curr);
    move_piece(wpawn4, WPAWN4, 4, 3); // from (4, 2) to (4, 3)

    // WPAWN5 (1)
    board_t* wpawn5 = copy_board(&curr);
    move_piece(wpawn5, WPAWN5, 5, 3); // from (5, 2) to (5, 3)

    // WPAWN6 (2)
    board_t* wpawn6_1 = copy_board(&curr);
    move_piece(wpawn6_1, WPAWN6, 6, 2); // from (6, 1) to (6, 2)
    board_t* wpawn6_2 = copy_board(&curr);
    move_piece(wpawn6_2, WPAWN6, 6, 3); // from (6, 1) to (6, 3)

    // WPAWN7 (2)
    board_t* wpawn7_1 = copy_board(&curr);
    move_piece(wpawn7_1, WPAWN7, 7, 2); // from (7, 1) to (7, 2)
    board_t* wpawn7_2 = copy_board(&curr);
    move_piece(wpawn7_2, WPAWN7, 7, 3); // from (7, 1) to (7, 3)

    // WROOK0 (1)
    board_t* wrook0 = copy_board(&curr);
    move_piece(wrook0, WROOK0, 0, 1); // from (0, 0) to (0, 1)

    // WKNIGHT0 (2)
    board_t* wknight0_1 = copy_board(&curr);
    move_piece(wknight0_1, WKNIGHT0, 2, 2); // from (1, 0) to (2, 2)
    board_t* wknight0_2 = copy_board(&curr);
    move_piece(wknight0_2, WKNIGHT0, 3, 1); // from (1, 0) to (3, 1)

    // WBISHOP0 (1)
    board_t* wbishop0 = copy_board(&curr);
    move_piece(wbishop0, WBISHOP0, 3, 1); // from (2, 0) to (3, 1)

    // WQUEEN (2)
    board_t* wqueen_1 = copy_board(&curr);
    move_piece(wqueen_1, WQUEEN, 3, 1); // from (3, 0) to (3, 1)
    board_t* wqueen_2 = copy_board(&curr);
    move_piece(wqueen_2, WQUEEN, 4, 1); // from (3, 0) to (4, 1)

    // WKING (3)
    board_t* wking_1 = copy_board(&curr);
    move_piece(wking_1, WKING, 3, 1); // from (4, 0) to (3, 1)
    board_t* wking_2 = copy_board(&curr);
    move_piece(wking_2, WKING, 4, 1); // from (4, 0) to (4, 1)
    board_t* wking_3 = copy_board(&curr);
    move_piece(wking_3, WKING, 5, 1); // from (4, 0) to (5, 1)

    // WBISHOP1 (1)
    board_t* wbishop1 = copy_board(&curr);
    move_piece(wbishop1, WBISHOP1, 4, 1); // from (2, 5) to (4, 1)

    // WKNIGHT1 (2)
    board_t* wknight1_1 = copy_board(&curr);
    move_piece(wknight1_1, WKNIGHT1, 4, 1); // from (6, 0) to (4, 1)
    board_t* wknight1_2 = copy_board(&curr);
    move_piece(wknight1_2, WKNIGHT1, 7, 2); // from (6, 0) to (7, 2)

    // BPAWN0 (1)
    board_t* bpawn0 = copy_board(&curr);
    move_piece(bpawn0, BPAWN0, 0, 4); // from (0, 5) to (0, 4)
    
    // BPAWN1 (2)
    board_t* bpawn1_1 = copy_board(&curr);
    move_piece(bpawn1_1, BPAWN1, 1, 5); // from (1, 6) to (1, 5)
    board_t* bpawn1_2 = copy_board(&curr);
    move_piece(bpawn1_2, BPAWN1, 1, 4); // from (1, 6) to (1, 4)

    // BPAWN2 (2)
    board_t* bpawn2_1 = copy_board(&curr);
    move_piece(bpawn2_1, BPAWN2, 2, 5); // from (2, 6) to (2, 5)
    board_t* bpawn2_2 = copy_board(&curr);
    move_piece(bpawn2_2, BPAWN2, 2, 4); // from (2, 6) to (2, 4)

    // BPAWN3 (1)
    board_t* bpawn3 = copy_board(&curr);
    move_piece(bpawn3, BPAWN3, 3, 4); // from (3, 5) to (3, 4)

    // BPAWN4 (1)
    board_t* bpawn4 = copy_board(&curr);
    move_piece(bpawn4, BPAWN4, 4, 4); // from (4, 5) to (4, 4)

    // BPAWN5 (1)
    board_t* bpawn5 = copy_board(&curr);
    move_piece(bpawn5, BPAWN5, 5, 4); // from (5, 5) to (5, 4)

    // BPAWN6 (2)
    board_t* bpawn6_1 = copy_board(&curr);
    move_piece(bpawn6_1, BPAWN6, 6, 5); // from (6, 6) to (6, 5)
    board_t* bpawn6_2 = copy_board(&curr);
    move_piece(bpawn6_2, BPAWN6, 6, 4); // from (6, 6) to (6, 4)

    // BPAWN7 (2)
    board_t* bpawn7_1 = copy_board(&curr);
    move_piece(bpawn7_1, BPAWN7, 7, 5); // from (7, 6) to (7, 5)
    board_t* bpawn7_2 = copy_board(&curr);
    move_piece(bpawn7_2, BPAWN7, 7, 4); // from (7, 6) to (7, 4)

    // BROOK0 (1)
    board_t* brook0 = copy_board(&curr);
    move_piece(brook0, BROOK0, 0, 6); // from (0, 7) to (0, 6)

    // BKNIGHT0 (2)
    board_t* bknight0_1 = copy_board(&curr);
    move_piece(bknight0_1, BKNIGHT0, 2, 5); // from (1, 7) to (2, 5)
    board_t* bknight0_2 = copy_board(&curr);
    move_piece(bknight0_2, BKNIGHT0, 3, 6); // from (1, 7) to (3, 6)

    // BBISHOP0 (1)
    board_t* bbishop0 = copy_board(&curr);
    move_piece(bbishop0, BBISHOP0, 3, 6); // from (2, 7) to (3, 6)

    // BQUEEN (2)
    board_t* bqueen_1 = copy_board(&curr);
    move_piece(bqueen_1, BQUEEN, 3, 6); // from (3, 7) to (3, 6)
    board_t* bqueen_2 = copy_board(&curr);
    move_piece(bqueen_2, BQUEEN, 4, 6); // from (3, 7) to (4, 6)

    // BKING (3)
    board_t* bking_1 = copy_board(&curr);
    move_piece(bking_1, BKING, 3, 6); // from (4, 7) to (3, 6)
    board_t* bking_2 = copy_board(&curr);
    move_piece(bking_2, BKING, 4, 6); // from (4, 7) to (4, 6)
    board_t* bking_3 = copy_board(&curr);
    move_piece(bking_3, BKING, 5, 6); // from (4, 7) to (5, 6)

    // BBISHOP1 (1)
    board_t* bbishop1 = copy_board(&curr);
    move_piece(bbishop1, BBISHOP1, 4, 6); // from (5, 7) to (4, 6)

    // BKNIGHT1 (2)
    board_t* bknight1_1 = copy_board(&curr);
    move_piece(bknight1_1, BKNIGHT1, 4, 6); // from (6, 7) to (4, 6)
    board_t* bknight1_2 = copy_board(&curr);
    move_piece(bknight1_2, BKNIGHT1, 7, 5); // from (6, 7) to (7, 5)

    move_list_t w_expected;
    board_t* w_expected_moves[24] = {wpawn0, wpawn1_1, wpawn1_2, wpawn2_1, wpawn2_2, wpawn3, wpawn4, 
                                    wpawn5, wpawn6_1, wpawn6_2, wpawn7_1, wpawn7_2, 
                                    wrook0, wknight0_1, wknight0_2, wbishop0, wqueen_1, wqueen_2, 
                                    wking_1, wking_2, wking_3, wbishop1, wknight1_1, wknight1_2};
    w_expected.moves = w_expected_moves;
    w_expected.num_moves = 24;

    move_list_t *w_actual = generate_all_moves(&game, WHITE);

    move_list_t b_expected;
    board_t* b_expected_moves[24] = {bpawn0, bpawn1_1, bpawn1_2, bpawn2_1, bpawn2_2, bpawn3, bpawn4, 
                                    bpawn5, bpawn6_1, bpawn6_2, bpawn7_1, bpawn7_2,
                                    brook0, bknight0_1, bknight0_2, bbishop0, bqueen_1, bqueen_2, 
                                    bking_1, bking_2, bking_3, bbishop1, bknight1_1, bknight1_2};
    b_expected.moves = b_expected_moves;
    b_expected.num_moves = 24;

    move_list_t *b_actual = generate_all_moves(&game, BLACK);

    // CMP boards
    test_result_t w_result = board_list_equals(&w_expected, w_actual);
    test_result_t b_result = board_list_equals(&b_expected, b_actual);

    // display results
    print_test_result(w_result, __func__);
    print_test_result(b_result, __func__);

    // free generated boards
    free(wpawn0);
    free(wpawn1_1);
    free(wpawn1_2);
    free(wpawn2_1);
    free(wpawn2_2);
    free(wpawn3);
    free(wpawn4);
    free(wpawn5);
    free(wpawn6_1);
    free(wpawn6_2);
    free(wpawn7_1);
    free(wpawn7_2);
    free(wrook0);
    free(wknight0_1);
    free(wknight0_2);
    free(wknight1_1);
    free(wknight1_2);
    free(wbishop0);
    free(wbishop1);
    free(wqueen_1);
    free(wqueen_2);
    free(wking_1);
    free(wking_2);
    free(wking_3);
    free(bpawn0);
    free(bpawn1_1);
    free(bpawn1_2);
    free(bpawn2_1);
    free(bpawn2_2);
    free(bpawn3);
    free(bpawn4);
    free(bpawn5);
    free(bpawn6_1);
    free(bpawn6_2);
    free(bpawn7_1);
    free(bpawn7_2);
    free(brook0);
    free(bknight0_1);
    free(bknight0_2);
    free(bknight1_1);
    free(bknight1_2);
    free(bbishop0);
    free(bbishop1);
    free(bqueen_1);
    free(bqueen_2);
    free(bking_1);
    free(bking_2);
    free(bking_3);
    destroy_move_list(w_actual);
    destroy_move_list(b_actual);
}