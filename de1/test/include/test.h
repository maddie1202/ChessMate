#ifndef TEST
#define TEST

#include <assert.h>
#include "../../include/game.h"

typedef struct test_result
{
    int passed;
    char message[80];
} test_result_t;

test_result_t return_test_result(int passed, char message[80]);
test_result_t board_equals(board_t* expected, board_t* actual);

#endif

