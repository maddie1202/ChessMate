#include "include/game.h"
#include "include/ai-move-generator.h"
#include "include/possible-move-generators.h"
#include "include/comms.h"
#include "include/HW.h"
#include <stdbool.h>
#include <stdio.h>

int lw_fd, sdram_fd;
void *lw_virtual, *sdram_virtual;

/*
 * Setup virtual addressing for SDRAM and LW bridge,
 * then initialize virtual addresses for buttons and switches,
 * then setup global settings for using libcurl for http requests.
 */
bool setup()
{
    if (!setup_hardware()) {
        printf("Hardware setup error\n");
        return false;
    }

    if (!init_hardware()) {
        printf("Netowrking init error\n");
        return false;
    }

    if (!init_networking()) {
        printf("Netowrking init error\n");
        return false;
    }

    return true;
}

/*
 * Cleanup all the virtual addressing dealing with the DE1.
 */
void teardown()
{
    teardown_hardware();
    cleanup_networking();
}

/*
 * Main state machine for playing a game with the app.
 */
void play_game()
{
    enum game_state state = WAIT_GAME;
    game_t *game = init_game();
    int game_id, seq_num;

    if (!setup()) return;

    while (true) {
        display_state(state);

        switch (state) {
            case WAIT_GAME: 
                printf("WAIT_GAME\n");
                if (start_new_game(&game_id)) {
                    send_ack_start_game();
                    state = WAIT_MOVE;
                } else if (resume_old_game(game, &game_id)) {
                    state = SEND_MOVE;
                }
                break;
            case WAIT_MOVE: 
                printf("WAIT_MOVE\n");
                printf("%d\n", game_id);
                if (receive_move(game, game_id)) {
                    state = SEND_MOVE;
                } else if (pause_game()) {
                    state = WAIT_GAME;
                }
                break;
            case SEND_MOVE:
                printf("SEND_MOVE\n");
                game->board = generate_ai_move(game, BLACK, 3);
                move_list_t *possible_player_moves = generate_all_moves(game, WHITE);

                if (in_checkmate(game->board, WHITE)) {
                    send_game_over_white_wins(game_id);
                    state = WAIT_GAME;
                } else if (in_checkmate(game->board, BLACK)) {
                    send_game_over_black_wins(game, game_id, seq_num);
                    state = WAIT_GAME;
                } else {
                    send_move(game, possible_player_moves, game_id, seq_num);
                    state = WAIT_MOVE;
                }
                break;
        }

        if (end_program()) break;
    }

    teardown();
}

int main()
{
    play_game();
    return 0;
}