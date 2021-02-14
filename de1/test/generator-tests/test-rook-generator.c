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

void test_rook_generator()
{
    rook_test0w();
    rook_test0b();
}

