/*
 * Author: Rafael Nicolas Trozzo
 */

#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

string blockGen(int height, int txCount, string prevBlockId, json& blockCh);
void merkleRootGen(string* TXids, int txCount, string& merkleRoot);
static unsigned int generateID(const char* str);
void generateTX(json& tx);

//recibe por linea de comandos: cantidad de bloques y cantidad de tx por bloque
int main(int argc, char* argv[])
{
	if (argc >= 2)
	{
		srand(time(NULL));
		int blockCount = atoi(argv[1]), txCount = atoi(argv[2]);
		json blockCh;
		string prevBlockId = "00000000";
		for (int i = 0; i < blockCount; i++)	//creo la blockchain
		{
			prevBlockId = blockGen(i, txCount, prevBlockId, blockCh);
		}
		ofstream blockChFile("blockChain.json");
		blockChFile << blockCh.dump(4);	  // indenta con 4 espacios para que se vea bien
		blockChFile.close();
	}
	else
	{
		cout << "Ingrese por linea de comandos: " << endl << "1. Numero de bloques" << endl;
		cout << "2. Numero de transacciones por bloque" << endl;
	}
	cout << "Programa finalizado, presione Enter para continuar" << endl;
	getchar();
	//char buff[9];
	//char buff2[9];
	//char buff3[9];
	//int num1 = generateID("A1243101");
	//int num2 = generateID("9AE3C9D9");
	//int num3 = generateID("B57D6C0A");
	//int num4 = generateID("750FACD9");
	//sprintf(buff, "%08X", num1);
	//sprintf(buff2, "%08X", num2);
	//string conc = buff;
	//conc += buff2;
	//sprintf(buff, "%08X", generateID(conc.c_str()));	//HTAB

	//sprintf(buff2, "%08X", num3);	//HTC
	//sprintf(buff3, "%08X", num4);	//HTD
	//conc = buff2;
	//conc += buff3;
	//sprintf(buff2, "%08X", generateID(conc.c_str()));	//HTCD

	//conc = buff;
	//conc += buff2;
	//sprintf(buff, "%08X", generateID(conc.c_str()));	//MERKLE ROOT

	//cout << "Hash: " << buff << endl;
	//getchar();
	return 0;
}

//blockGen: crea un bloque
//recibe altura del bloque, cantidad de transacciones, id del bloque previo y la blockchain donde agrega el bloque
//devuelve ID del bloque creado
string blockGen(int height, int txCount, string prevBlockId, json& blockCh)
{
	json block;
	string merkleRoot = "";
	int newID;
	char newIDstr[9] = " ";
	char newTXID[9] = " ";
	try
	{
		json* TX = new json[txCount];
		string* TXids = new string[txCount + 1]; // guardo lugar para un TXid mas en caso que haya que copiar el ultimo valor si txCount es impar
		for (int i = 0; i < txCount; i++)
		{
			generateTX(TX[i]);	// TX cualesquiera
			TXids[i] = TX[i]["txid"];	// guardo el id de la tx hasheado para generar el merkletree
		}
		block["nTx"] = txCount;
		block["height"] = height;					// altura actual
		block["nonce"] = rand() % 10000;			// esto no hace falta en este tp
		merkleRootGen(TXids, txCount, merkleRoot);		// guarda el merkle root en merkleRoot
		block["merkleroot"] = merkleRoot;
		block["previousblockid"] = prevBlockId;
		newID = generateID(block.dump().c_str());	// aplico la funcion magica a todo el bloque
		sprintf(newIDstr, "%08X", newID);			// paso a hex coded ASCII para que siempre tengan el mismo tamaño
		block["blockid"] = newIDstr;
		for (int i = 0; i < txCount; i++)
		{
			block["tx"][i] = TX[i];
		}
		delete[] TX;
		delete[] TXids;
		blockCh += block;
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
	}
	return string(newIDstr);	//construye el string para devolver
}

void generateTX(json& tx)
{
	int ntxin = rand() % 5 + 1;
	int ntxout = rand() % 5 + 1;
	int newID;
	string conc = "";
	char buff[9];
	for (int i = 0; i < ntxin; i++)
	{
		sprintf(buff, "%08X", rand() % 5000);
		tx["vin"][i]["blockid"] = buff;
		sprintf(buff, "%08X", rand() % 5000);
		tx["vin"][i]["txid"] = buff;
		conc += buff;
		sprintf(buff, "%08X", rand() % 5000);
		tx["vin"][i]["signature"] = buff;
		tx["vin"][i]["outputIndex"] = rand() % 5;
	}
	tx["nTxin"] = ntxin;
	newID = generateID(conc.c_str());		//para generar el newID, calculo el "hash" de la concatenacion de txids
	sprintf(buff, "%08X", newID);			//a eso lo paso a hex coded ASCII
	tx["txid"] = buff;						//voila
	for (int i = 0; i < ntxout; i++)
	{
		tx["vout"][i]["publicid"] = "91218912199121891218";
		tx["vout"][i]["amount"] = (rand() % 5000) / 2000;	//cantidad random de bitcoins, maximo 2.5 porque nos hacemos los millonarios pero no tanto
	}
	tx["nTxout"] = ntxout;
}

void merkleRootGen(string* TXids, int txCount, string& merkleRoot)
{
	char nodestr[9];
	if ((txCount % 2) > 0)  // si hay una cantidad impar de transacciones, copio la ultima
	{
		TXids[txCount] = TXids[txCount - 1];
	}
	for (unsigned int i = 0; i < txCount / 2.0; i++)
	{
		sprintf(nodestr, "%08X", generateID((TXids[2 * i] + TXids[2 * i + 1]).c_str()));	// genero cada nodo como el "hash" de dos
		TXids[i] = nodestr;
	}
	if (txCount > 2)
	{
		merkleRootGen(TXids, ceil(txCount / 2.0), merkleRoot);
	}
	else
	{
		merkleRoot = TXids[0];
	}
}

//static unsigned int generateID(unsigned char * str)
static unsigned int generateID(const char* str)
{
	unsigned int ID = 0;
	int c;
	while (c = *str++)
	{
		ID = c + (ID << 6) + (ID << 16) - ID;
	}
	return ID;
}
