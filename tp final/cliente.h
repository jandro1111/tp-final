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
    std::string client(std::string request,int puerto,int option,int cant,std::string id, const char* data,int& imgui);//recive la request q hace el puerto y devuelve un string con lo que recivio

private:
    nlohmann::json j;//aca guardo la response

};

