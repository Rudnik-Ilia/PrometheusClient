#include <iostream>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>

// g++ --std=c++17 test_server.cpp -lcurl
// sudo apt-get update
// sudo apt-get install libcurl4-openssl-dev


int main()
{   
    CURL *curl = NULL;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5000/");
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        const char *metricsData = "my_metric 42\n";

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, metricsData);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) 
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return 0;
}


// struct ResponseStorage
// {
//     char *data;
//     size_t size;
// };


// size_t WriteCallBack(void *content, size_t size, size_t nmemb, void *user_ptr)
// {
//     size_t real_size = nmemb * size;

//     struct ResponseStorage *storage = (struct ResponseStorage *)user_ptr;

//     user_ptr = (char*)realloc(storage->data, (storage->size + real_size + 1));

//     if(user_ptr == NULL)
//     {
//         fprintf(stderr, "Allocation error!");
//         return 0;
//     }

//     // storage->data = ptr;
//     memcpy(storage->data, content, real_size);
//     storage->size += real_size;

//     return real_size;
// }



    // struct ResponseStorage chunk;
    // chunk.data = (char*)malloc(1);
    // chunk.size = 0;

    // curl = curl_easy_init();
    // if(curl == NULL)
    // {
    //     fprintf(stderr, "HTTP request is failed!\n");
    // }

    // curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5000/");
    // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
    // curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk.data);

    // res = curl_easy_perform(curl);

    // if(res != CURLE_OK)
    // {
    //     fprintf(stderr, "Error: %s !\n", curl_easy_strerror(res));
    //     return -1;
    // }

    // printf("%ld\n", sizeof(chunk));