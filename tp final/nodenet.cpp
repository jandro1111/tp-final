#include "nodenet.h"

void nodenet::createnode(bool nodefull,int port,std::string ip) {//que imgui verifique que el puerto del servidor sea correcto
	struct nodo nuevo;
	nuevo.nodofull = nodefull;
	//std::string request[6] = { "blockpost/","transactionpost/","merkleblockpost/","filterpost/","getblocks","getblockheader" };
	if (nodefull == true) {//le pongo solo las opciones a las que puede acceder el request de arriba ya esta puesto en nodenet, queda aca de referencia nomas
		nuevo.options.push_back(0);//0
		nuevo.options.push_back(1);//1
		nuevo.options.push_back(2);//2
		nuevo.options.push_back(4);//3
	}
	else {//le pongo solo las opciones a las que puede acceder
		nuevo.options.push_back(1);//0
		nuevo.options.push_back(3);//1
		nuevo.options.push_back(5);//2
	}
	nuevo.ip = ip;
	nuevo.pserver = port;
	nuevo.pcliente = port + 1;
	nuevo.vecinos.clear();
	ips.push_back(ip);//agrego la nueva ip a la lista
	puertos.push_back(port);//agrego el nuevo puerto a la lista
	nodos.push_back(nuevo);//ahora si, meto el nuevo nodo en la net
}

//
bool nodenet::canconect(int node1, int node2) {
	if ((node1 < nodos.capacity()) && (node2 < nodos.capacity())) {
		if (nodos[node1].nodofull==true|| nodos[node2].nodofull == true) {
			return true;
		}
		else {//son los dos spv
			return false;
		}
	}
	else {//el nodo al cual queres comunicar no existe
		return false;
	}
}
//
bool nodenet::conectnode(int node1,int node2) {
	if (canconect(node1, node2)) {//si es un input valido los "conecto"
		nodos[node1].vecinos.push_back(node2);
		nodos[node2].vecinos.push_back(node1);
	}
	else {//esos nodos o no existen o no se pueden conectar
		return false;
	}
}
//
std::vector<int> nodenet::getvecinosnodo(int nodo) {
	std::vector<int> null;
	null.clear();
	if (nodo < nodos.capacity()) {
		return nodos[nodo].vecinos;
	}
	else {
		return null;
	}
}
//
std::string nodenet::clientconect(int node1, int node2, int option,int cant,std::string id,int bloque,int ntx,int& imgui) {
	std::string aux = "";
	blockchain algo2("ejemplo.json");
	bool isconected = false;
	for (std::vector<int>::iterator it = nodos[node1].vecinos.begin(); it != nodos[node1].vecinos.end(); ++it) {
		if (*it==node2) {//si esta conectado
			isconected = true;
			break;
		}
	}
	if (canconect(node1,node2)||isconected==false) {//si se puede conectar
		if (nodos[node1].nodofull == true && nodos[node2].nodofull == false && (option == 2 || option == 3)) {//un nodo full no puede pedirle bloque o getblock a un nodo spv
			return "esta opcion no es valida para estos nodos";
		}else{
			if (option < nodos[node1].options.capacity()) {//si la opcion existe
				try
				{
					std::string data = "";
					switch (nodos[node1].options[option]) {
					case 0:
						data = algo2.getblock(bloque).dump();
						break;
					case 1:
						data = algo2.sendtx(cant, id);
						break;
					case 2:
						data = algo2.makemerkleblock(bloque, ntx);
						break;
					case 3:
						data = "{\"Key\" : \"pubkey1\"}";
						break;
					default:
						break;
					}
					aux = nodos[node2].ip;
					aux += request[nodos[node1].options[option]];
					aux=nodos[node1].client.client(aux, nodos[node2].pserver, nodos[node1].options[option], cant, id, data.c_str(),imgui);
				}
				catch (std::exception& e)
				{
					std::cerr << e.what() << std::endl<<"no se pudo conectar a ese nodo"<<std::endl;
				}
			}
			else {
				return "esa opcion no existe";
			}

		}
	}
	else {//no se puede conectar con ese nodo
		return "estos nodos no se pueden conectar";
	}
	return aux;
}