#ifndef TEST
#define TEST

#include <assert.h>
#include "../../include/game.h"
#include "../../include/possible-move-generators.h"

typedef struct test_result
{
    int passed;
    char message[80];
} test_result_t;

test_result_t return_test_result(int passed, char message[80]);
test_result_t board_equals(board_t* expected, board_t* actual);
test_result_t board_list_equals(move_list_t* expected, move_list_t* actual);

#endif

