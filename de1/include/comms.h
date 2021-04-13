#include "game.h"
#include "possible-move-generators.h"
#include <stdbool.h>
#include <stdio.h>

enum game_state {WAIT_GAME, WAIT_MOVE, SEND_MOVE};

struct string {
    char *ptr;
    size_t len;
};

bool init_hardware();
bool start_new_game(int *game_id);
bool resume_old_game(game_t *game, int *game_id, int *seq_num);
void display_state(enum game_state state);
bool receive_recent_move(game_t *game, int game_id, int *expected_seq_num);
bool pause_game();

void send_ack_start_game(game_t *game, int game_id);
void send_move(game_t *game, move_list_t *possible_player_moves, int game_id, int *seq_num);
void send_game_over_black_wins(game_t* game, int game_id, int *seq_num);
void send_game_over_white_wins(int game_id);

bool init_networking();
void cleanup_networking();

bool end_program();

bool send_post_request(char *url, char *body);
bool send_put_request(char *url, char *body);
bool send_get_request(char *url, char **result);