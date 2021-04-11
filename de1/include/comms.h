#include "game.h"
#include "possible-move-generators.h"
#include <stdbool.h>

enum game_state {WAIT_GAME, WAIT_MOVE, SEND_MOVE};

void init_hardware();
bool start_new_game(int *game_id);
bool resume_old_game(game_t *game, int *game_id);
void display_state(enum game_state state);
bool receive_move(game_t *game, int game_id);
bool pause_game();

void send_ack_start_game();
void send_move(game_t *game, move_list_t *possible_player_moves, int game_id);
void send_game_over_black_wins(game_t* game, int game_id);
void send_game_over_white_wins(int game_id);