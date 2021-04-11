#include <stdio.h>
#include "include/test-game.h"
#include "include/test.h"
#include "../include/game.h"
#include "include/test-possible-move-generators.h"
#include "include/test-ai-move-generator.h"
#include <string.h>
#include <stdbool.h>
#include "../include/possible-move-generators.h"
#include <stdlib.h>
#include "../include/HW.h"

int lw_fd, sdram_fd;
void *lw_virtual, *sdram_virtual;

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
                return test_result(false, "Mismatch");
            }
        }
    }

    return test_result(true, "");
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
        if (matches == 0) {
            return test_result(false, "Incorrect move(s)");
        }
    }

    return test_result(true, "");
}

void print_test_result(test_result_t result, const char* test)
{
    if (result.passed) printf("%-20s: PASSED\n", test);
    else printf("%-20s: FAILED (%s)\n", test, result.message);
}

int main()
{
    // Create virtual memory access to the FPGA light-weight bridge
    lw_fd = -1;
    sdram_fd = -1;
    if ((lw_fd = open_physical (lw_fd)) == -1) return -1;
    if ((lw_virtual = map_physical (lw_fd, lw_bridge_offset, lw_bridge_span)) == NULL) return -1;

    // Create virtual memory access to the SDRAM
    if ((sdram_fd = open_physical (sdram_fd)) == -1) return -1;
    if ((sdram_virtual = map_physical (sdram_fd, sdram_offset, sdram_span)) == NULL) return -1;

    if (lw_virtual == NULL || sdram_virtual == NULL) return -1;

    printf("Running test_game...\n");
    test_game();
    printf("\n\n");

    printf("Running test_possible_moves...\n");
    test_possible_moves();
    printf("\n\n");

    printf("Running test_ai_move_generator...\n");
    test_ai_move_generator();
    printf("\n\n");

    unmap_physical (lw_virtual, lw_bridge_span);
    close_physical (lw_fd);
    unmap_physical (sdram_virtual, sdram_span);
    close_physical (sdram_fd);

    return 0;
}