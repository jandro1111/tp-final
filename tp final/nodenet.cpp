#include "nodenet.h"

void nodenet::createnode(bool nodefull,int port,std::string ip) {//que imgui verifique que el puerto del servidor sea correcto
	struct nodo nuevo;
	nuevo.nodofull = nodefull;
	//std::string request[6] = { "blockpost/","transactionpost/","merkleblockpost/","filterpost/","getblocks","getblockheader" };
	if (nodefull == true) {//le pongo solo las opciones a las que puede acceder el request de arriba ya esta puesto en nodenet, queda aca de referencia nomas
		nuevo.options.push_back(0);
		nuevo.options.push_back(1);
		nuevo.options.push_back(2);
		nuevo.options.push_back(4);
	}
	else {//le pongo solo las opciones a las que puede acceder
		nuevo.options.push_back(1);
		nuevo.options.push_back(3);
		nuevo.options.push_back(5);
	}
	nuevo.ip = ip;
	nuevo.pserver = port;
	nuevo.pcliente = nuevo.pserver + 1;
	nuevo.ips = ips;//le paso el vector de los ips que ya tengo
	nuevo.svports = puertos;//le paso el vector con los 
	ips.push_back(ip);//agrego la nueva ip a la lista
	puertos.push_back(port);//agrego el nuevo puerto a la lista

	std::vector<std::string> aux;
	std::vector<int> aux1;
	for (int i = 0; i < nodos.capacity(); i++) {//actualizo la info de los demas nodos
		for (std::vector<std::string>::iterator it = ips.begin(); it != ips.end();it++) {
			if (nodos[i].ip==*it) {//si es la ip de este nodo la salteo
			}
			else {
				aux.push_back(*it);
			}
		}
		for (std::vector<int>::iterator it = puertos.begin(); it != puertos.end(); it++) {
			if (nodos[i].pserver == *it) {//si es la ip de este nodo la salteo
			}
			else {
				aux1.push_back(*it);
			}
		}
		nodos[i].ips = aux;
		nodos[i].svports = aux1;
		aux.empty();
		aux1.empty();
	}
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
std::string nodenet::clientconect(int node1, int node2, int option,int cant,std::string id,int bloque,int ntx) {
	std::string aux = "";
	blockchain algo2("ejemplo.json");
	if (canconect(node1,node2)) {//si se puede conectar
		if (nodos[node1].nodofull == true && nodos[node2].nodofull == false && (option == 0 || option == 4)) {//un nodo full no puede pedirle bloque o getblock a un nodo spv
			return "esta opcion no es valida para estos nodos";
		}else{
			if (option < nodos[node1].options.capacity()) {//si la opcion existe
				try
				{
					std::string data = "";
					//nodos[node2].server...
					switch (nodos[node1].options[option]) {//despues sacar afuera y cambiar i x option
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
					aux=nodos[node1].client.client(aux, nodos[node2].pserver, nodos[node1].options[option], cant, id, data.c_str());
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