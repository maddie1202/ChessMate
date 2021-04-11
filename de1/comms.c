#include "include/comms.h"
#include "include/game.h"
#include "include/possible-move-generators.h"
#include "include/HW.h"
#include <stdbool.h>

#define KEY0 1
#define KEY1 2
#define KEY2 4
#define KEY3 8

volatile int *pb_edgecapture_addr;
volatile int *leds_addr;
volatile int *switches_addr;

void init_hardware()
{
    pb_edgecapture_addr = (volatile int *)(lw_virtual + pb_edgecapture_offset);
    leds_addr = (volatile int *)(lw_virtual + leds_offset);
    switches_addr = (volatile int *)(lw_virtual + switches_offset);
    *pb_edgecapture_addr = 0;
}

bool start_new_game(int *game_id)
{
    long PBreleases = *pb_edgecapture_addr;

    if (PBreleases == KEY0) {
        *game_id = *switches_addr;
        return true;
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
