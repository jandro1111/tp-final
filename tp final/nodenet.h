#pragma once
#include "Header.h"
#include "cliente.h"
#include "server.h"
#include "blockchain.h"
struct nodo {
	//server Server;//arreglar constructor por defecto
	cliente client;
	bool nodofull;
	std::list <std::string> options ;
	int pserver;
	int pcliente;
};

class nodenet
{
public:
	void createnode(bool nodofull);//crea un nuevo nodo
private:
	std::list<nodo> nodos;
	std::list<int> puertos;
};

