#include "include/comms.h"
#include "include/game.h"
#include "include/possible-move-generators.h"
#include <stdbool.h>

void init_hardware()
{
    *PB_EDGECAPTURE = 0;
}

bool start_new_game(int *game_id)
{
    long PBreleases = *PB_EDGECAPTURE;

    // Display the state of the change register on red LEDs
    *RLEDs = PBreleases;
    if (PBreleases)
    {
        // Delay, so that we can observe the change on the LEDs
        for (int i = 0; i < 10000000; i++);
        *PB_EDGECAPTURE = 0; //reset the changes for next round
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
