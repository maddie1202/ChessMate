#ifndef TEST
#define TEST

#include <assert.h>
#include "../game.h"

typedef struct test_result
{
    int passed;
    char message[80];
} t_test_result;

t_test_result return_test_result(int passed, char message[80]);

#endif

