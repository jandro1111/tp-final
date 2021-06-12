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
//std::string cliente::client(std::string request,int puerto,int option,int cant,std::string id, const char* data,int& imgui)
//{
//    struct MemoryStruct chunk;
//    chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
//    chunk.size = 0;    /* no data at this point */
//    CURL* curl;
//    CURLcode res;
//    blockchain algo("ejemplo.json");
//    switch (option) {
//         case 4:
//             request += "?block_id=";
//             request += id;
//             request += "&count=";
//             request += std::to_string(cant);
//             break;
//         case 5:
//             request += "?block_id=";
//             request += id;
//             request += "&count=";
//             request += std::to_string(cant);
//             break;
//         default:
//            break;
//    }
    
//    curl = curl_easy_init();
//    if (curl) {
//        curl_easy_setopt(curl, CURLOPT_PORT, puerto);
//        /* example.com is redirected, so we tell libcurl to follow redirection */
//        switch (option) {
//        case 0:
//        case 1:
//        case 2://falta hacer
//        case 3:
//            curl_easy_setopt(curl, CURLOPT_POST, 1L);
//            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(data));
//            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
//            break;
//        case 4:
//        case 5:
//            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
//            break;
//        default:
//            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//            break;
//        }
//        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//        curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
//        curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);
//        /* send all data to this function  */
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

//        /* we pass our 'chunk' struct to the callback function */
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
//        //hasta aca anda bien

//        /* Perform the request, res will get the return code */

//        //
//        try {
//            res = curl_easy_perform(curl);
//            imgui = CANCONECT;
//            std::cout << res << std::endl;
//            /* Check for errors */
//            if (res != CURLE_OK) {
//                fprintf(stderr, "curl_easy_perform() failed: %s\n",
//                    curl_easy_strerror(res));
//            }
//            else {
//                printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
//            }
//            //aca guardar en algun lado chunk

//            std::string aux = "";
//            for (int i = 0; i < chunk.size; i++) {
//                aux += *(chunk.memory + i);
//            }
//            std::cout << aux << std::endl;

//            j = nlohmann::json::parse(aux);
//            /* always cleanup */
//            curl_easy_cleanup(curl);
//            free(chunk.memory);
//            curl_global_cleanup();
//            std::cout << j << std::endl;
//            imgui = RESPONSEOK;
//            return aux;
//        }
//        catch (std::exception& e) {
//            std::cerr << e.what() << std::endl << "no se pudo conectar a ese nodo" << std::endl;
//            imgui = RESPONSEFAIL;
//        }
//    }
//    else {
//        return "could not perform curl";
//    }
//}

/*HTTP/1.1 200 OK
Date: Date (Ej: Tue, 04 Sep 2018 18:21:19 GMT)
Location: 127.0.0.1/prueba.txt
Cache-Control: max-age=30
Expires: Date + 30s (Ej: Tue, 04 Sep 2018 18:21:49 GMT)
Content-Length: 34
Content-Type: json; charset=iso-8859-1


{"status": true,"result": "ok" }



*///respuesta de prueba
 
void cliente::configClient()
{
	this->curl = curl_easy_init();
	this->multiHandle = curl_multi_init();
}


nlohmann::json cliente::client(std::string request,int puerto,int option,int cant,std::string id, const char* data,int& imgui)
{
    struct MemoryStruct chunk;
    chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */
    blockchain algo("ejemplo.json");
    switch (option) {
         case 4:
             request += "?block_id=";
             request += id;
             request += "&count=";
             request += std::to_string(cant);
             break;
         case 5:
             request += "?block_id=";
             request += id;
             request += "&count=";
             request += std::to_string(cant);
             break;
         default:
             request += "conect";
            break;
    }


	if ((curl != NULL) && (multiHandle != NULL))
	{
		//Attacheo el easy handle para manejar una coneccion no bloqueante.
		curl_multi_add_handle(multiHandle, curl);

		//Seteamos URL FOLLOWLOCATION y los protocolos a utilizar.
		curl_easy_setopt(curl, CURLOPT_PORT, puerto);

        switch (option) {
        case 0:
        case 1:
        case 2://falta hacer
        case 3:
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(data));
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
            break;
        case 4:
        case 5:
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
            break;
        default:
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            break;
        }
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
        curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);
        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
        //hasta aca anda bien

        try {
            //Se realiza ahora un perform no bloqueante.
		    output = curl_multi_perform(multiHandle, &stillRunning);
            //imgui = CANCONECT;

            std::cout << output << std::endl;
            /* Check for errors */

            if (output != CURLE_OK)
		    {
			    std::cerr << "curl_multi_perform() failed: " << curl_multi_strerror(output) << std::endl;
				curl_multi_cleanup(multiHandle);
				curl_easy_cleanup(curl);
                curl_global_cleanup();
                return "could not perform curl";
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

            
            /* always cleanup */
            //Si el request de CURL fue exitoso entonces twitter devuelve un JSON
		    //con toda la informacion de los tweets que le pedimos
		    if (stillRunning == 0)	//Si el proceso termino, se escribe el json
		    {
				//Cleanup al final.
				curl_multi_cleanup(multiHandle);
				curl_easy_cleanup(curl);
                curl_global_cleanup();
                std::cout << "TERMINO" << std::endl;
                if(!aux.empty())
                    j = nlohmann::json::parse(aux);
                imgui = 1;
		    }

            free(chunk.memory);
            std::cout << "J" << j << std::endl;
           //imgui = RESPONSEOK;
            return j;
        }
        catch (std::exception& e) {
            std::cerr << e.what() << std::endl << "HOLAAA no se pudo conectar a ese nodo" << std::endl;
            //imgui = RESPONSEFAIL;
        }
    }
    else {
        return "could not perform curl";
    }
}