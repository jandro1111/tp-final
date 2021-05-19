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
	//nlohmann::json gettx(nlohmann::json);
private:
	nlohmann::json j;// el blockchain
};

