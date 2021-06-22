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
	void setnTx(int block, int ); //recibe el numero de bloque y el nTx
	void setnonce(int block, int);//recibe el numero de bloque y el nonce
	void setpreviousblockid(int block, std::string);//recibe el numero de bloque y el previousblockid
	void setnTxin(int block, int tx, int);//recibe el numero de bloque, de transaccion y de ntxin
	void setnTxout(int block, int tx, int);//recibe el numero de bloque, de transaccion y de ntxout
	void settxid(int block, int tx, std::string);//recibe el numero de bloque, de transaccion y de txid
	void setoutputIndex(int block, int tx, int ntxin, int outputindex);//recibe el numero de bloque, de transaccion,de txid y el outputindex
	void setsignature(int block, int tx, int ntxin, std::string signature);//recibe el numero de bloque, de transaccion,de txid y la signature
	void setamount(int block, int tx, int ntxout, int amount);//recibe el numero de bloque, de transaccion,de txid y el amount
	void setpublicid(int block, int tx, int ntxout, std::string publicid);//recibe el numero de bloque, de transaccion,de txid y el publicid
	void setvout(int block, int tx, int amount, std::string publicid);//recive el numero de bloque, de transaccion, el nuevo amount y el nuevo publicid
	void setvin(int block, int tx, std::string blockid, int out, std::string signature, std::string txid);//recive el numero de bloque, de transaccion, el nuevo vin
	void settx(int block, std::string txid);//recive el numero de bloque y el id de la nueva transaccion, esta empieza con 0 ntxin ntxout
	void setblock(std::string blockid,  std::string merkleroot, int nonce);//setea un nuevo bloque con 0 transacciones
	void setchallenge(int c);//setea el challenge
//otras funciones
	std::string calculatemerkleroot(int);//recibe el numero del bloque del cual va a querer calcular el merkleroot, devuelve null si es un numero de bloque no valido
	std::vector<std::vector<std::string>> calculatemerkletree(int);
	void calculatecantblocks(void); //calcula cuantos bloques hay en la blockchain
	std::string getblocks(int cant,int in);//la cantidad de bloques a gettear y a partir de que bloque buscar
	int searchid(std::string id);//busca en q numero de bloque esta ese id, devuelve -1 si el bloque no existe
	std::string getheader(int cant, int indi);//busca el numero de ids a partir de ese id
	nlohmann::json makeheader(int i);//recive el indice del bloque del cual hay q armar el header
	std::string makemerkleblock(int block,int tx);//recibe el numero de bloque del cual va a hacer el merkleblock y el numero de transaccion, si no existe devuelve null
	std::string makemerklepath(int block, int tx);//recibe el numero de bloque del cual va a hacer el merklepath y el numero de transaccion, si no existe devuelve null
	std::string sendtx(int amount, std::string publicid);//crea una transaccion para mandarla
	bool mine(std::string tx, int ntx,bool newblock);//recive las transacciones a minar, y la cantidad de estas, devuelve si se pudo minar el bloque
private:
	nlohmann::json j;// el blockchain
	int cantblocks;
	int cant0 = 10;//el challenge 13 es lo maximo que pude cumplir
};
std::string generateID(std::string);
std::string int2hex(int);
std::string hasheo(std::string);
const char* hex_char_to_bin(char c);
std::string GetHexFromBin(std::string sBinary);
using namespace CryptoPP;
bool txverifier(ECDSA<ECP, SHA256>::PublicKey publicKey,std::string signature,std::string tx);//verifica las transacciones
