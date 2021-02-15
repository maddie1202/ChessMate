#ifndef TEST
#define TEST

#include <assert.h>
#include "../../include/game.h"
#include "../../include/possible-move-generators.h"
#include <stdbool.h>

typedef struct test_result
{
    bool passed;
    char message[80];
} test_result_t;

test_result_t return_test_result(int passed, char message[80]);
test_result_t board_equals(board_t* expected, board_t* actual);
test_result_t board_list_equals(move_list_t* expected, move_list_t* actual);
void print_test_result(test_result_t result, const char* test);

#endif

