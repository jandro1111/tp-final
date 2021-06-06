#pragma once
#include "Header.h"
#include "cliente.h"
#include "server.h"
#include "blockchain.h"
struct nodo {
	//server Server;//arreglar constructor por defecto
	cliente client;
	bool nodofull;
	std::vector <int> options ;//tiene el indice de las request a las q puedo acceder dependiendo del tipo de nodo
	int pserver;//tiene q ser par
	int pcliente;
	std::string ip;
	std::vector<std::string> ips;
	std::vector<int> svports;//
};

class nodenet
{
public:
	void createnode(bool nodofull,int port,std::string ip);//crea un nuevo nodo
	bool canconect(int node1, int node2);//verifica que estos nodos se puedan conectar
	std::string clientconect(int node1, int node2, int option, int cant, std::string id,int bloque ,int ntx,int& imgui);//devuelve el mensaje si se pudo conectar, o que no se pudo conectar, el primer nodo es el del cliente el segundo el server
	//cant y id es tanto para el post de una tx como para el getblocks/blockheader, y data  ntx y bloque es para hacer un post de ese item
private:
	std::vector<nodo> nodos;
	std::vector<std::string> ips;
	std::vector<int> puertos;
	std::string request[6] = { "blockpost/","transactionpost/","merkleblockpost/","filterpost/","getblocks","getblockheader" };
};

