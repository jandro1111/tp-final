#pragma once
#include "Header.h"
#include "blockchain.h"
//structs
struct MemoryStruct {
    char* memory;
    size_t size;
};
// external funcs
static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp);
//cliente
class cliente
{
public:
    std::string nodo(std::string request);//recive la request q hace y devuelve un string con lo que recivio

private:
    nlohmann::json j;//aca guardo la response

};

