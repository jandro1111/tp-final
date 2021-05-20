#include "blockchain.h"
using namespace std;
//	generate id	//
unsigned int generateID(const char* str)
{
	unsigned int ID = 0;
	int c;
	while (c = *str++)
	{
		ID = c + (ID << 6) + (ID << 16) - ID;
	}
	return ID;
}
//  hexcoded ascii
template< typename T >
std::string int_to_hex(T i)
{
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(sizeof(T) * 2)
        << std::hex << i;
    return stream.str();
}
string int2hex(int num) {
    return int_to_hex(num);
}

//
//	operaciones del bloque	//
//	constructor
blockchain::blockchain(std::string file)
{
    std::ifstream doc;
    std::string linea,aux;
    doc.open(file.c_str());
    if (doc.is_open()) {
        while (getline(doc, linea)) {
            aux += linea;
        }
        doc.close();//termine de leer
        cout << "lo pude abrir" << endl;
    }
    else {
        cout << "el archivo no existe" << endl;
    }
    try {
        j = nlohmann::json::parse(aux);
    }
    catch (std::exception& e)
    {
        //Muestro si hubo un error de la libreria
        cout<< e.what() <<endl<<"el json no se pudo parsear bien";
    }
}
//	destructor
blockchain::~blockchain() {

}
// get block
nlohmann::json blockchain::getblock(int cant)
{
    try {
        return j[cant];
    }
    catch (...) {
        return NULL;
    }
}
//  get blockid
std::string blockchain::getblockid(nlohmann::json block)
{
    return  block.at("blockid");
}
//  get nTx
int blockchain::getnTx(nlohmann::json block)
{
    return block.at("nTx");
}
//  get height
int blockchain::getheight(nlohmann::json block)
{
    return block.at("height");
}
//  get previousblockid
std::string blockchain::getpreviousblockid(nlohmann::json block)
{
    return block.at("previousblockid");
}
//  get merkleroot
std::string blockchain::getmerkleroot(nlohmann::json block)
{
    return block.at("merkleroot");
}
//  get nonce
int blockchain::getnonce(nlohmann::json block)
{
    return block.at("nonce");
}
//  get tx
nlohmann::json blockchain::gettx(nlohmann::json block,int cant)
{
    try {
        return block.at("tx")[cant];
    }
    catch (...) {
        return NULL;
    }
}
//  get vin
nlohmann::json blockchain::getvin(nlohmann::json tx,int cant)
{
    try {
        return tx.at("vin")[cant];
    }
    catch (...) {
        return NULL;
    }
}
//  get vout
nlohmann::json blockchain::getvout(nlohmann::json tx,int cant)
{
    try {
        return tx.at("vout")[cant];
    }
    catch (...) {
        return NULL;
    }
}
//  get txid
std::string blockchain::gettxid(nlohmann::json tx)
{
    return tx.at("txid");
}
//  get ntxin
int blockchain::getnTxin(nlohmann::json tx)
{
    return tx.at("nTxin");
}
//  get ntxout
int blockchain::getnTxout(nlohmann::json tx)
{
    return tx.at("nTxout");
}
//  get public id
std::string blockchain::getpublicid(nlohmann::json vout)
{
    return vout.at("publicid");
}
//  get amount
int blockchain::getamount(nlohmann::json vout)
{
    return  vout.at("amount");
}
//  get signature
string blockchain::getsignature(nlohmann::json vin)
{
    return  vin.at("signature");
}
//  get outputindex
int blockchain::getoutputIndex(nlohmann::json vin)
{
    return  vin.at("outputIndex");
}

//  calcula el merkle root
std::string blockchain::calculatemerkleroot(int num)
{
    string merkleroot="";
    nlohmann::json bloque;
    if (getblock(num) == NULL) {//ese bloque no existe
        return merkleroot;
    }
    bloque = getblock(num);


    return merkleroot;
}
// 
