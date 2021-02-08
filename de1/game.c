#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "test/test.h"

board* init_board()
{
    board* new_game = malloc(sizeof(board));

    return new_game;
}

board* copy_board(board* original)
{

}

void init_and_set_board()
{
    current_game = init_board();
}

void destroy_board()
{
    free(current_game);
}

t_test_result board_equals(board* expected, board* actual)
{
    t_test_result result;

    if (expected == NULL || actual == NULL) return return_test_result(0, "NULL parameter(s)");

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((*expected)[i][j] != (*actual)[i][j]) {
                return return_test_result(0, "Mismatch");
            }
        }
    }

    return return_test_result(1, "Passed");
}