#include "include/comms.h"
#include "include/game.h"
#include "include/possible-move-generators.h"
#include "include/HW.h"
#include <stdbool.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include "frozen/frozen.h"

#define PUT_GAME_RESULT_URL "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/updateresult"
#define POST_GAME_RESULT_URL "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/postresult"

#define POST_AI_MOVE_URL "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/makeboard"
#define POST_POTENTIAL_PLAYER_MOVES_URL "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/createmoves"

#define GET_RECENT_BOARD_URL "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/getlatestboard/%d"

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
bool resume_old_game(game_t *game, int *game_id, int *seq_num)
{
    long PBreleases = *pb_edgecapture_addr;

    if (PBreleases == KEY2) {
        *game_id = *switches_addr;
        *pb_edgecapture_addr = 0;

        char url[256];
        sprintf(url, GET_RECENT_BOARD_URL, *game_id);

        char *result;
        if (!send_get_request(url, &result)) return false;

        char *placements;
        int board_id, game_id, sequenceNumber, wrook0_moved, wrook1_moved, brook0_moved, brook1_moved, wking_moved, bking_moved;
        json_scanf(result, strlen(result), 
            "{boardID: %d, placements: %Q, gameID: %d, sequenceNumber: %d, \
            wrookO_moved: %d, wrookI_moved: %d, brookO_moved: %d, brookI_moved: %d, \
            wking_moved: %d, bking_moved: %d}", &board_id, &placements, &game_id, &sequenceNumber, 
            &wrook0_moved, &wrook1_moved, &brook0_moved, &brook1_moved, &wking_moved, &bking_moved);

        if (strcmp("", placements)) {
            printf("RESUME OLD GAME: placements is empty\n");
            return false;
        }
    
        free(game->board);
        game->board = read_board(placements);
        game->wrook0_has_moved = wrook0_moved;
        game->wrook1_has_moved = wrook1_moved;
        game->brook0_has_moved = brook0_moved;
        game->brook1_has_moved = brook1_moved;
        game->wking_has_moved = wking_moved;
        game->bking_has_moved = bking_moved;

        *seq_num = sequenceNumber;

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
bool receive_recent_move(game_t *game, int game_id, int *expected_seq_num)
{
    char url[256];
    sprintf(url, GET_RECENT_BOARD_URL, game_id);

    char *result;
    if (!send_get_request(url, &result)) return false;

    char *placements;
    int board_id, game_ID, sequenceNumber, wrook0_moved, wrook1_moved, brook0_moved, brook1_moved, wking_moved, bking_moved;
    json_scanf(result, strlen(result), 
        "{boardID: %d, placements: %Q, gameID: %d, sequenceNumber: %d, \
        wrookO_moved: %d, wrookI_moved: %d, brookO_moved: %d, brookI_moved: %d, \
        wking_moved: %d, bking_moved: %d}", &board_id, &placements, &game_ID, &sequenceNumber, 
        &wrook0_moved, &wrook1_moved, &brook0_moved, &brook1_moved, &wking_moved, &bking_moved);

    if (strcmp("", placements)) {
        printf("RECEIVE RECENT MOVE: placements is empty\n");
        return false;
    }

    if (sequenceNumber != *expected_seq_num) {
        return false;
    } else {
        free(game->board);
        game->board = read_board(placements);
        game->wrook0_has_moved = wrook0_moved;
        game->wrook1_has_moved = wrook1_moved;
        game->brook0_has_moved = brook0_moved;
        game->brook1_has_moved = brook1_moved;
        game->wking_has_moved = wking_moved;
        game->bking_has_moved = bking_moved;

        (*expected_seq_num)++;

        return true;
    }
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
 * Send init board
 */
void send_ack_start_game(game_t *game, int game_id)
{
    int seq_num = 0;
    send_move(game, NULL, game_id, &seq_num);
}

/*
 * Post both the AI move and the list of potential player moves to the database.
 */
void send_move(game_t *game, move_list_t *possible_player_moves, int game_id, int *seq_num)
{
    // AI move
    char body[512];
    char placements[256];
    format_board(game->board, placements);
    sprintf(body, "placements=%s&gameID=%d&sequenceNum=%d&wrookO_moved=%d&wrookI_moved=%d&wking_moved=%d&brookO_moved=%d&brookI_moved=%d&bking_moved=%d", 
            placements, game_id, *seq_num, game->wrook0_has_moved, game->wrook1_has_moved, game->wking_has_moved, 
            game->brook0_has_moved, game->brook1_has_moved, game->bking_has_moved);
    send_post_request(POST_AI_MOVE_URL, body);

    (*seq_num)++;

    // list of potential player moves
    if (possible_player_moves == NULL) return;

    char *body2 = malloc(257 * possible_player_moves->num_moves + 30);

    char *placements2 = format_move_list(possible_player_moves);
    sprintf(body2, "placements=%s", placements2);
    send_post_request(POST_POTENTIAL_PLAYER_MOVES_URL, body2);

    free(placements2);
    free(body2);
}

/*
 * If the AI has won, we need to send over our winning move, and then update 
 * the game result accordingly.
 */
void send_game_over_black_wins(game_t* game, int game_id, int *seq_num)
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

void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(s->len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
    size_t new_len = s->len + size*nmemb;
    s->ptr = realloc(s->ptr, new_len+1);
    if (s->ptr == NULL) {
      fprintf(stderr, "realloc() failed\n");
      exit(EXIT_FAILURE);
    }
    memcpy(s->ptr+s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size*nmemb;
}

/*
 * Sends a get request to the given url.  Saves the data received in a file???
 */
bool send_get_request(char *url, char **result)
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        struct string s;
        init_string(&s);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", 
                curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            return false;
        }

        printf("%s\n", s.ptr);

        *result = malloc(strlen(s.ptr) + 1);
        strcpy(*result, s.ptr);

        free(s.ptr);

        /* always cleanup */
        curl_easy_cleanup(curl);

        return true;
    }

    return false;
}
