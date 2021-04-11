#include "include/comms.h"
#include "include/game.h"
#include "include/possible-move-generators.h"
#include "include/HW.h"
#include <stdbool.h>

volatile int *pb_edgecapture_addr;
volatile int *leds_addr;

void init_hardware()
{
    pb_edgecapture_addr = (volatile int *)(lw_virtual + pb_edgecapture_offset);
    leds_addr = (volatile int *)(lw_virtual + leds_offset);
    *pb_edgecapture_addr = 0;
}

bool start_new_game(int *game_id)
{
    long PBreleases = *pb_edgecapture_addr;

    // Display the state of the change register on red LEDs
    *leds_addr = PBreleases;
    if (PBreleases)
    {
        // Delay, so that we can observe the change on the LEDs
        for (int i = 0; i < 10000000; i++);
        *pb_edgecapture_addr = 0; //reset the changes for next round
    }

    return false;
}

bool resume_old_game(game_t *game, int *game_id)
{
    return false;
}

void display_state(enum game_state state)
{

}

bool receive_move(game_t *game, int game_id)
{
    return false;
}

bool pause_game()
{
    return false;
}

void send_ack_start_game()
{

}

void send_move(game_t *game, move_list_t *possible_player_moves, int game_id)
{

}

void send_game_over_black_wins(game_t* game, int game_id)
{

}

void send_game_over_white_wins(int game_id)
{

}
