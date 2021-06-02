#include "cliente.h"
using boost::asio::ip::tcp;

static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    char* ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}
//


//
std::string cliente::nodo(std::string request,int puerto)
{
    struct MemoryStruct chunk;
    chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_PORT, puerto);
        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
        curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);
        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
        //hasta aca anda bien

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        std::cout << res << std::endl;
        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        }
        else {
            printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
        }
        //aca guardar en algun lado chunk

        std::string aux = "";
        for (int i = 0; i < chunk.size; i++) {
            aux += *(chunk.memory + i);
        }
        std::cout << aux << std::endl;

        j = nlohmann::json::parse(aux);
        /* always cleanup */
        curl_easy_cleanup(curl);
        free(chunk.memory);
        curl_global_cleanup();
        std::cout << j << std::endl;
        return aux;
    }
    else {
        return "could not perform curl";
    }
}

/*HTTP/1.1 200 OK
Date: Date (Ej: Tue, 04 Sep 2018 18:21:19 GMT)
Location: 127.0.0.1/prueba.txt
Cache-Control: max-age=30
Expires: Date + 30s (Ej: Tue, 04 Sep 2018 18:21:49 GMT)
Content-Length: 34
Content-Type: json; charset=iso-8859-1


{"status": true,"result": "ok" }



*///respuesta de prueba
 