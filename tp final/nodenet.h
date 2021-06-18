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
	std::vector<int> vecinos;//vector con sus nodos vecinos
	bool minero;
	ECDSA<ECP, SHA256>::PrivateKey pvkey;
	ECDSA<ECP, SHA256>::PublicKey publicKey;
};

class nodenet
{
public:
	nodo getnodo(int nodo);//busca ese nodo
	int getcantnodos();//busca cuantos nodos hay
	void createnode(bool nodofull,int port,std::string ip);//crea un nuevo nodo
	bool canconect(int node1, int node2);//verifica que estos nodos se puedan conectar
	std::vector<int>getvecinosnodo(int nodo);//te trae un vector con los indices de los vectores a los que esta conectado
	bool conectnode(int node1, int node2);//"conecta" dos nodos
	nlohmann::json clientconect(int node1, int node2, int option, int cant, std::string id,int bloque ,int ntx,int& imgui);//devuelve el mensaje si se pudo conectar, o que no se pudo conectar, el primer nodo es el del cliente el segundo el server
	//cant y id es tanto para el post de una tx como para el getblocks/blockheader, y data  ntx y bloque es para hacer un post de ese item

	std::vector<nodo> nodos;
private:
	int cantnodos=0;
	
	std::vector<std::string> ips;
	std::vector<int> puertos;
	std::string request[6] = { "blockpost/","transactionpost/","merkleblockpost/","filterpost/","getblocks","getblockheader" };
};

