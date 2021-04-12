#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "include/game.h"
#include "include/possible-move-generators.h"
#include <string.h>

#define PUT_GAME_RESULT_URL "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/updateresult"
#define POST_GAME_RESULT_URL "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/postresult"

#define POST_AI_MOVE_URL "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/makeboard"
#define POST_POTENTIAL_PLAYER_MOVES_URL "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/createmoves"

struct MyData
{
    char data[80];
};

board_t *read_board(char *str_ptr)
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

void format_board(board_t *board, char *str_ptr)
{
    sprintf(str_ptr, "%d", (*board)[0][0]);
    for (int i = 1; i < 64; i++) {
        sprintf(str_ptr, "%s %d", str_ptr, (*board)[i / 8][i % 8]);
    }
}

char *format_move_list(move_list_t *moves)
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
 
int main(void)
{
    board_t reference = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    game_t *game = init_game();
    move_list_t *moves = generate_all_moves(game, WHITE);
    char *str_ptr = format_move_list(moves);
    if (str_ptr == NULL) printf("AGGGHHHHH\n");
    printf("%s\n", str_ptr);

    CURL *curl;
    CURLcode res;

    /* get a curl handle */ 
    curl = curl_easy_init();
    if (curl) {
        /* First set the URL that is about to receive our POST. This URL can
        just as well be a https:// URL if that is what should receive the
        data. */ 
        char *body = malloc(257 * moves->num_moves + 30);
        char *placements = format_move_list(moves);
        sprintf(body, "placements=%s", placements);
        printf("%s\n", body);
        curl_easy_setopt(curl, CURLOPT_URL, POST_POTENTIAL_PLAYER_MOVES_URL);
        /* Now specify the POST data */ 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    
        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);
        /* Check for errors */ 
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", 
                curl_easy_strerror(res));
        }
    
        /* always cleanup */ 
        curl_easy_cleanup(curl);
    }
    
    return 0;

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ POST BOARD ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

    // CURL *curl;
    // CURLcode res;

    // /* get a curl handle */ 
    // curl = curl_easy_init();
    // if (curl) {
    //     /* First set the URL that is about to receive our POST. This URL can
    //     just as well be a https:// URL if that is what should receive the
    //     data. */ 
    //     char body[512];
    //     char placements[256];
    //     format_board(reference, placements);
    //     sprintf(body, "placements=%s&gameID=75&sequenceNum=0", placements);
    //     printf("%s\n", body);
    //     curl_easy_setopt(curl, CURLOPT_URL, POST_AI_MOVE_URL);
    //     /* Now specify the POST data */ 
    //     curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    
    //     /* Perform the request, res will get the return code */ 
    //     res = curl_easy_perform(curl);
    //     /* Check for errors */ 
    //     if (res != CURLE_OK) {
    //         fprintf(stderr, "curl_easy_perform() failed: %s\n", 
    //             curl_easy_strerror(res));
    //     }
    
    //     /* always cleanup */ 
    //     curl_easy_cleanup(curl);
    // }
    
    // return 0;

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ GET ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

    // CURL *curl = curl_easy_init();
    // struct MyData this;
    // if (curl) {
    //     curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
        
    //     /* pass pointer that gets passed in to the
    //         CURLOPT_READFUNCTION callback */
    //     curl_easy_setopt(curl, CURLOPT_READDATA, &this);
        
    //     curl_easy_perform(curl);

    //     printf("OUR PRINT STATEMENTS\n\n\n");
    //     printf("%s\n", this.data);
    // }

    // return 0;

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PUT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

    // CURL *curl;
    // CURLcode res;

    // /* get a curl handle */ 
    // curl = curl_easy_init();
    // if (curl) {
    //     /* First set the URL that is about to receive our POST. This URL can
    //     just as well be a https:// URL if that is what should receive the
    //     data. */ 
    //     curl_easy_setopt(curl, CURLOPT_URL, PUT_GAME_RESULT_URL);
    //     /* Now specify the POST data */ 
    //     curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "userID=User2&gameID=75&result=0");
    //     curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    
    //     /* Perform the request, res will get the return code */ 
    //     res = curl_easy_perform(curl);
    //     /* Check for errors */ 
    //     if (res != CURLE_OK) {
    //         printf("curl_easy_perform() failed: %s\n", 
    //             curl_easy_strerror(res));
    //     }
    
    //     /* always cleanup */ 
    //     curl_easy_cleanup(curl);
    // }

    // return 0;
}