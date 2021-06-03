#include "nodenet.h"

void nodenet::createnode(bool nodefull) {
	struct nodo nuevo;
	std::string request[6] = { "blockpost/","transactionpost/","merkleblockpost/","filterpost/","getblocks","getblockheader" };
	if (nodefull = true) {
		nuevo.options.push_back(request[0]);
		nuevo.options.push_back(request[1]);
		nuevo.options.push_back(request[2]);
		nuevo.options.push_back(request[4]);
		if (puertos.empty()) {//si no hay ningun puerto
			puertos.push_back(PUERTOINI);
			nuevo.pcliente = PUERTOINI;
			nuevo.pserver = PUERTOINI + 1;
		}
		else {
			nuevo.pcliente = puertos.back()+1;
			puertos.push_back(puertos.back() + 1);
			nuevo.pserver = puertos.back() + 1;
			puertos.push_back(puertos.back() + 1);
		}
		
	}
	else {
		nuevo.options.push_back(request[1]);
		nuevo.options.push_back(request[3]);
		nuevo.options.push_back(request[5]);
		if (puertos.empty()) {//si no hay ningun puerto
			puertos.push_back(PUERTOINI);
			nuevo.pcliente = PUERTOINI;
			nuevo.pserver = PUERTOINI + 1;
		}
		else {
			nuevo.pcliente = puertos.back() + 1;
			puertos.push_back(puertos.back() + 1);
			nuevo.pserver = puertos.back() + 1;
			puertos.push_back(puertos.back() + 1);
		}
	}
}
//