#include <stdio.h>
#include "test-game.h"
#include "test.h"
#include "../game.h"
#include <string.h>

test_result_t return_test_result(int passed, char message[80])
{
    test_result_t result;

    result.passed = passed;
    strcpy(result.message, message);
    
    return result;
}

test_result_t board_equals(board_t* expected, board_t* actual)
{
    test_result_t result;

    if (expected == NULL || actual == NULL) return return_test_result(0, "NULL parameter(s)");

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((*expected)[i][j] != (*actual)[i][j]) {
                return return_test_result(0, "Error: Mismatch");
            }
        }
    }

    return return_test_result(1, "Passed");
}

int test_game()
{
    test_init_board();
    test_copy_board();
}

int main()
{
    test_game();
    return 0;
}