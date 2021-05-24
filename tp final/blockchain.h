#pragma once
#include"Header.h"


class blockchain
{
public:
	blockchain(std::string);
	~blockchain();
	//geters
	nlohmann::json getblock(int);//si se puede acceder al bloque devuelve ese bloque, si no devuelve nulljson
	std::string getblockid(nlohmann::json);//recibe el json de getblock devuelve el blockid
	int getnTx(nlohmann::json);//recibe el json de getblock devuelve el nTx
	int getheight(nlohmann::json);//recibe el json de getblock devuelve el height
	std::string getpreviousblockid(nlohmann::json);//recibe el json de getblock devuelve el previousblockid
	std::string getmerkleroot(nlohmann::json);//recibe el json de getblock devuelve el merkleroot
	int getnonce(nlohmann::json);//recibe el json de getblock devuelve el nonce
	nlohmann::json gettx(nlohmann::json, int);//recibe el json de getblock devuelve el tx si existe, si no devuelve nulljson
	nlohmann::json getvin(nlohmann::json,int);//recibe el json de gettx devuelve el vin si existe, si no devuelve nulljson
	nlohmann::json getvout(nlohmann::json,int);//recibe el json de gettx devuelve el vout si existe, si no devuelve nulljson
	std::string gettxid(nlohmann::json);//recibe el json de gettx devuelve el txid
	int getnTxin(nlohmann::json);//recibe el json de gettx devuelve el ntxvin
	int getnTxout(nlohmann::json);//recibe el json de gettx devuelve el ntxvout
	std::string getpublicid(nlohmann::json);//recibe el json de getvout devuelve el publicid
	int getamount(nlohmann::json);//recibe el json de getvout devuelve el amount
	std::string getsignature(nlohmann::json);//recibe el json de getvin devuelve el signature
	int getoutputIndex(nlohmann::json);//recibe el json de getvin devuelve el outputindex
	int getcantblock(void);//dice cuantos bloques hay en el blockchain
	//seters
	void setblockid(int block, std::string blockid);//recibe el numero de bloque y el id
	void setheight(int block, std::string height);//recibe el numero de bloque y el height
	void setmerkleroot(int block, std::string);//recibe el numero de bloque y el merkleroot
	void setnTx(int block, int);//recibe el numero de bloque y el nTx
	void setnonce(int block, int);//recibe el numero de bloque y el nonce
	void setpreviousblockid(int block, std::string);//recibe el numero de bloque y el previousblockid
	void setnTxin(int block, int tx, int);//recibe el numero de bloque, de transaccion y de ntxin
	void setnTxout(int block, int tx, int);//recibe el numero de bloque, de transaccion y de ntxout
	void settxid(int block, int tx, std::string);//recibe el numero de bloque, de transaccion y de txid
//otras funciones
	std::string calculatemerkleroot(int);//recibe el numero del bloque del cual va a querer calcular el merkleroot, devuelve null si es un numero de bloque no valido
	void calculatecantblocks(void);//calcula cuantos bloques hay en la blockchain
private:
	nlohmann::json j;// el blockchain
	int cantblocks;
};
unsigned int generateID(const char* str);
std::string int2hex(int);
