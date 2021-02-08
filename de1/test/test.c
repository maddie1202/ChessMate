#include <stdio.h>
#include "test-game.h"
#include "test.h"

t_test_result return_test_result(int passed, char message[80])
{
    t_test_result result;

    result.passed = passed;
    result.message = message;
    
    return result;
}

int test_game()
{
    test_init_board();
}

int main()
{
    test_game();
    return 0;
}