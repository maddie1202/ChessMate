#include "include/comms.h"
#include "include/game.h"
#include "include/possible-move-generators.h"
#include "include/HW.h"
#include <stdbool.h>
#include <curl/curl.h>
#include <string.h>

#define PUT_GAME_RESULT_URL "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/updateresult"

#define KEY0 1
#define KEY1 2
#define KEY2 4
#define KEY3 8

volatile int *pb_edgecapture_addr;
volatile int *leds_addr;
volatile int *switches_addr;

bool init_hardware()
{
    pb_edgecapture_addr = (volatile int *)(lw_virtual + pb_edgecapture_offset);
    leds_addr = (volatile int *)(lw_virtual + leds_offset);
    switches_addr = (volatile int *)(lw_virtual + switches_offset);
    *pb_edgecapture_addr = 0;

    return true;
}

bool init_networking()
{
    curl_global_init(CURL_GLOBAL_ALL);
    return true;
}

void cleanup_networking()
{
    curl_global_cleanup();
}

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

bool receive_move(game_t *game, int game_id)
{
    return false;
}

bool pause_game()
{
    long PBreleases = *pb_edgecapture_addr;

    if (PBreleases == KEY1) {
        *pb_edgecapture_addr = 0;
        return true;
    } 

    return false;
}

void send_ack_start_game()
{

}

void send_move(game_t *game, move_list_t *possible_player_moves, int game_id, int seq_num)
{
    
}

void send_game_over_black_wins(game_t* game, int game_id, int seq_num)
{

}

void send_game_over_white_wins(int game_id)
{
    char body[80];
    sprintf(body, "{\"gameId\": %d, \"result\": 1}", game_id);
    if (send_put_request(PUT_GAME_RESULT_URL, body)) {
        printf("Success!\n");
    } else {
        printf("Failure :(");
    }
}

bool end_program()
{
    long PBreleases = *pb_edgecapture_addr;

    if (PBreleases == KEY3) {
        *pb_edgecapture_addr = 0;
        return true;
    } 

    return false;
}

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
            return false;
        }
    
        /* always cleanup */ 
        curl_easy_cleanup(curl);
        return true;
    }

    return false;
}

bool send_put_request(char *url, char *body)
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
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    
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
