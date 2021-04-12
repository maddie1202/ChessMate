#include "include/comms.h"
#include "include/game.h"
#include "include/possible-move-generators.h"
#include "include/HW.h"
#include <stdbool.h>
#include <curl/curl.h>
#include <string.h>

#define PUT_GAME_RESULT_URL "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/updateresult"
#define POST_GAME_RESULT_URL "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/postresult"

#define POST_AI_MOVE_URL "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/makeboard"
#define POST_POTENTIAL_PLAYER_MOVES_URL "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/createmoves"

#define KEY0 1
#define KEY1 2
#define KEY2 4
#define KEY3 8

volatile int *pb_edgecapture_addr;
volatile int *leds_addr;
volatile int *switches_addr;

/*
 * Setup the virtual addressing for the pushbuttons, leds, and switches on the DE1.
 */
bool init_hardware()
{
    pb_edgecapture_addr = (volatile int *)(lw_virtual + pb_edgecapture_offset);
    leds_addr = (volatile int *)(lw_virtual + leds_offset);
    switches_addr = (volatile int *)(lw_virtual + switches_offset);
    *pb_edgecapture_addr = 0;

    return true;
}

/*
 * Setup libcurl global settings.
 */
bool init_networking()
{
    curl_global_init(CURL_GLOBAL_ALL);
    return true;
}

/* 
 * Cleanup liburl global settings.
 */
void cleanup_networking()
{
    curl_global_cleanup();
}

/*
 * Formats the board from the database into a board_t
 */
static board_t *read_board(char *str_ptr)
{
    board_t *board = malloc(sizeof(board_t));
    int i = 0;

    // get 1st token
    char *token = strtok(str_ptr, " ");

    // board chars are separated by spaces
    while (token != NULL) {
        (*board)[i / 8][i % 8] = atoi(token);
        i++;
        token = strtok(NULL, " ");
    }

    print_board(board);

    return board;
}

/*
 * Formats the board_t into a string to write to the database
 */
static void format_board(board_t *board, char *str_ptr)
{
    sprintf(str_ptr, "%d", (*board)[0][0]);
    for (int i = 1; i < 64; i++) {
        sprintf(str_ptr, "%s %d", str_ptr, (*board)[i / 8][i % 8]);
    }
}

/*
 * Formats the move_list_t into a string to write to the database
 */
static char *format_move_list(move_list_t *moves)
{
    if (moves->num_moves == 0) return NULL;

    char *str_ptr = malloc(257 * moves->num_moves); // 257 for boards and commas between

    char board_str[256];
    format_board(moves->moves[0], board_str);
    sprintf(str_ptr, "%s", board_str);

    for (int i = 1; i < moves->num_moves; i++) {
        char board_str[256];
        format_board(moves->moves[i], board_str);
        sprintf(str_ptr, "%s,%s", str_ptr, board_str);
    }

    return str_ptr;
}

/*
 * To start a new game, the player enters the game ID given on the 
 * app on the switches and presses KEY0 on the DE1.
 */
bool start_new_game(int *game_id)
{
    long PBreleases = *pb_edgecapture_addr;

    if (PBreleases == KEY0) {
        *game_id = *switches_addr;
        *pb_edgecapture_addr = 0;
        return true;
    } 

    return false;
}

/*
 * To resume a game, the player has to enter the game ID on the switches, 
 * and press KEY2 to confirm the input.
 * We will need to wait for the player move.
 */
bool resume_old_game(game_t *game, int *game_id)
{
    long PBreleases = *pb_edgecapture_addr;

    if (PBreleases == KEY2) {
        *game_id = *switches_addr;
        *pb_edgecapture_addr = 0;
        return true;
    } 

    return false;
}

void display_state(enum game_state state)
{

}

/*
 * Poll the DB until the player makes a new move in the game.
 */
bool receive_move(game_t *game, int game_id)
{
    return false;
}

/*
 * Player presses KEY1 to indicate that theyhave paused the game on the app.
 * Ensure that we have sent the latest AI move before pausing.
 */
bool pause_game()
{
    long PBreleases = *pb_edgecapture_addr;

    if (PBreleases == KEY1) {
        *pb_edgecapture_addr = 0;
        return true;
    } 

    return false;
}

/*
 * Send init board???
 */
void send_ack_start_game()
{

}

/*
 * Post both the AI move and the list of potential player moves to the database.
 */
void send_move(game_t *game, move_list_t *possible_player_moves, int game_id, int seq_num)
{
    // AI move

    // list of potential player moves

}

/*
 * If the AI has won, we need to send over our winning move, and then update 
 * the game result accordingly.
 */
void send_game_over_black_wins(game_t* game, int game_id, int seq_num)
{
    // send black's winning move
    send_move(game, NULL, game_id, seq_num);

    // update game result to 0 to indicate player lost
    char body[80];
    sprintf(body, "gameId=%d&result=0", game_id);
    if (send_put_request(PUT_GAME_RESULT_URL, body)) {
        printf("Successfully sent game over: black wins!\n");
    } else {
        printf("Failed to send game over: black wins\n");
    }
}

/*
 * If the player has won, all we need to do is update the game status.
 */
void send_game_over_white_wins(int game_id)
{
    // update game result to 1 to indicate player won
    char body[80];
    sprintf(body, "gameId=%d&result=1", game_id);
    if (send_put_request(PUT_GAME_RESULT_URL, body)) {
        printf("Successfully sent game over: white wins!\n");
    } else {
        printf("Failed to send game over: white wins\n");
    }
}

/* 
 * If the player presses KEY3 on the board, cleanup everything and exit the program. 
 */
bool end_program()
{
    long PBreleases = *pb_edgecapture_addr;

    if (PBreleases == KEY3) {
        *pb_edgecapture_addr = 0;
        return true;
    } 

    return false;
}

/*
 * Sends a post request to the given url containing the given body.
 */
bool send_post_request(char *url, char *body)
{
    CURL *curl;
    CURLcode res;

    /* get a curl handle */ 
    curl = curl_easy_init();
    if (curl) {
        /* First set the URL that is about to receive our POST. This URL can
        just as well be a https:// URL if that is what should receive the
        data. */ 
        curl_easy_setopt(curl, CURLOPT_URL, url);
        /* Now specify the POST data */ 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    
        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);
        /* Check for errors */ 
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", 
                curl_easy_strerror(res));
                curl_easy_cleanup(curl);
            return false;
        }
    
        /* always cleanup */ 
        curl_easy_cleanup(curl);
        return true;
    }

    return false;
}

/*
 * Sends a put request to the given url containing the given body.
 */
bool send_put_request(char *url, char *body)
{
    CURL *curl;
    CURLcode res;

    /* get a curl handle */ 
    curl = curl_easy_init();
    if (curl) {
        /* First set the URL that is about to receive our PUT. This URL can
        just as well be a https:// URL if that is what should receive the
        data. */ 
        curl_easy_setopt(curl, CURLOPT_URL, url);
        /* Now specify the PUT data */ 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    
        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);
        /* Check for errors */ 
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", 
                curl_easy_strerror(res));
                curl_easy_cleanup(curl);
            return false;
        }
    
        /* always cleanup */ 
        curl_easy_cleanup(curl);
        return true;
    }

    return false;
}

/*
 * Sends a get request to the given url.  Saves the data received in a file???
 */
bool send_get_request(char *url)
{
    CURL *curl;
    CURLcode res;
 
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);
        /* Check for errors */ 
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));
            return false;
        }
    
        /* always cleanup */ 
        curl_easy_cleanup(curl);
        return true;
    }

    return false;
}
