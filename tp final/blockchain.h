#pragma once
#include"Header.h"


class blockchain
{
public:
	blockchain(std::string);
	~blockchain();
	nlohmann::json getblock(int);//si se puede acceder al bloque devuelve ese bloque, si no devuelve null
	std::string getblockid(nlohmann::json);//recibe el json de getblock devuelve el blockid
	int getnTx(nlohmann::json);//recibe el json de getblock devuelve el nTx
	int getheight(nlohmann::json);//recibe el json de getblock devuelve el height
	std::string getpreviousblockid(nlohmann::json);//recibe el json de getblock devuelve el previousblockid
	std::string getmerkleroot(nlohmann::json);//recibe el json de getblock devuelve el merkleroot
	int getnonce(nlohmann::json);//recibe el json de getblock devuelve el nonce
	nlohmann::json gettx(nlohmann::json, int);//recibe el json de getblock devuelve el tx si existe, si no devuelve null
	nlohmann::json getvin(nlohmann::json,int);//recibe el json de gettx devuelve el vin si existe, si no devuelve null
	nlohmann::json getvout(nlohmann::json,int);//recibe el json de gettx devuelve el vout si existe, si no devuelve null
	std::string gettxid(nlohmann::json);//recibe el json de gettx devuelve el txid
	int getnTxin(nlohmann::json);//recibe el json de gettx devuelve el ntxvin
	int getnTxout(nlohmann::json);//recibe el json de gettx devuelve el ntxvout
	std::string getpublicid(nlohmann::json);//recibe el json de getvout devuelve el publicid
	int getamount(nlohmann::json);//recibe el json de getvout devuelve el amount
	std::string getsignature(nlohmann::json);//recibe el json de getvin devuelve el signature
	int getoutputIndex(nlohmann::json);//recibe el json de getvin devuelve el outputindex
	std::string calculatemerkleroot(int);//recibe el numero del bloque del cual va a querer calcular el merkleroot, devuelve null si es un numero de bloque no valido
private:
	nlohmann::json j;// el blockchain
};
unsigned int generateID(const char* str);
std::string int2hex(int);