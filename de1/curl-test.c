#include <stdio.h>
#include <curl/curl.h>

#define PUT_GAME_RESULT_URL "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/updateresult"
#define POST_GAME_RESULT_URL "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/postresult"

struct MyData
{
    char data[80];
};
 
int main(void)
{
    CURL *curl = curl_easy_init();
    struct MyData this;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
        
        /* pass pointer that gets passed in to the
            CURLOPT_READFUNCTION callback */
        curl_easy_setopt(curl, CURLOPT_READDATA, &this);
        
        curl_easy_perform(curl);

        printf("OUR PRINT STATEMENTS\n\n\n");
        printf("%s\n", this.data);
    }

    return 0;

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