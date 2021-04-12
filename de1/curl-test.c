#include <stdio.h>
#include <curl/curl.h>

#define PUT_GAME_RESULT_URL "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/updateresult"
#define POST_GAME_RESULT_URL "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/postresult"
 
int main(void)
{
    CURL *curl;
    CURLcode res;

    /* get a curl handle */ 
    curl = curl_easy_init();
    if (curl) {
        /* First set the URL that is about to receive our POST. This URL can
        just as well be a https:// URL if that is what should receive the
        data. */ 
        curl_easy_setopt(curl, CURLOPT_URL, PUT_GAME_RESULT_URL);
        /* Now specify the POST data */ 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"userID\": 1, \"gameID\": 7, \"result\": 1}");
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    
        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);
        /* Check for errors */ 
        if (res != CURLE_OK) {
            printf("curl_easy_perform() failed: %s\n", 
                curl_easy_strerror(res));
        }
    
        /* always cleanup */ 
        curl_easy_cleanup(curl);
    }

    return 0;
}