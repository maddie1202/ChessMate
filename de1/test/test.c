#include <stdio.h>
#include "include/test-game.h"
#include "include/test.h"
#include "../include/game.h"
#include "include/test-possible-move-generators.h"
#include <string.h>

test_result_t test_result(int passed, char message[80])
{
    test_result_t result;

    result.passed = passed; // 1 is pass
    strcpy(result.message, message);
    
    return result;
}

test_result_t board_equals(board_t* expected, board_t* actual)
{
    if (expected == NULL || actual == NULL) return test_result(0, "NULL parameter(s)");

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((*expected)[i][j] != (*actual)[i][j]) {
                return test_result(0, "Error: Mismatch");
            }
        }
    }

    return test_result(1, "Passed");
}

test_result_t board_list_equals(move_list_t* expected, move_list_t* actual)
{
    if (expected == NULL || actual == NULL) return test_result(0, "NULL parameter(s)");

    if (expected->num_moves != actual->num_moves) return test_result(0, "Incorrect number of moves");

    for (int i = 0; i < expected->num_moves; i++) {
        int matches = 0;
        for (int j = 0; j < actual->num_moves; j++) {
            if (board_equals(expected->moves[i], actual->moves[j]).passed == 1) {
                matches++;
                break;
            }
        }
        if (matches == 0) return test_result(0, "Incorrect move(s)");
    }

    return test_result(1, "Passed");
}

int main()
{
    test_game();

    test_pawn_generator();
    return 0;
}