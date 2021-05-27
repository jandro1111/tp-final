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
template <typename I> std::string n2hexstr(I w, size_t hex_len = sizeof(I) << 1) {
    static const char* digits = "0123456789ABCDEF";
    std::string rc(hex_len, '0');
    for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
        rc[i] = digits[(w >> j) & 0x0f];
    return rc;
}
string int2hex(int num) {
    return n2hexstr(num,sizeof(num)*2);
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
        //cout << "lo pude abrir" << endl;
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
    calculatecantblocks();
}
//	destructor
blockchain::~blockchain() {

}
//calculateblocks
void blockchain::calculatecantblocks() {
    int i = 0;
    for (i; j[i] != nulljson; ++i) {

    }
    cantblocks = i;
}
//get cantblocks
int blockchain::getcantblock() {
    return cantblocks;
}
// get block
nlohmann::json blockchain::getblock(int cant)
{
    try {
        return j[cant];
    }
    catch (...) {
        return nulljson;
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
        return  nulljson;
    }
}
//  get vin
nlohmann::json blockchain::getvin(nlohmann::json tx,int cant)
{
    try {
        return tx.at("vin")[cant];
    }
    catch (...) {
        return nulljson;
    }
}
//  get vout
nlohmann::json blockchain::getvout(nlohmann::json tx,int cant)
{
    try {
        return tx.at("vout")[cant];
    }
    catch (...) {
        return nulljson;
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
std::vector<std::string> blockchain::calculatemerkletree(int num)
{
    int itx = 0;
    int itxin = 0;
    string aux = "";

    vector<string> temp;
    vector<string> temp2;
    vector<string> fullMerkleTree; //Contiene todo el merkle tree, empezando por hojas y terminando por el root

    list<string> hojas;
    nlohmann::json bloque, tx, vin;
    if (getblock(num) == nulljson) {//ese bloque no existe
        return fullMerkleTree;
    }
    bloque = getblock(num);
    itx = getnTx(bloque);
    for (int i = 0; i < itx; ++i) {//creo un lista con las hojas
        tx = gettx(bloque, i);
        itxin = getnTxin(tx);
        for (int j = 0; j < itxin; j++) {
            vin = getvin(tx, j);
            aux += gettxid(vin);
        }
        aux = int2hex(generateID(aux.c_str()));

        aux.resize(8);
        aux.shrink_to_fit();
        hojas.push_back(aux);
        if (((itx % 2) != 0) && (i == itx - 1)) {//si estoy en la ultima hoja, y esta es impar, la vuelvo a agregar a la lista
            hojas.push_back(aux);
        }
        aux = "";
    }

    // hasta aca calcula las hojas

    for (std::list<string>::iterator it = hojas.begin(); it != hojas.end(); ++it) {
        temp.push_back(*it);
        fullMerkleTree.push_back(*it);
    }

    string newID;
    if (temp.size() == 1) {
        cout << "Block only has one trsnaction!" << endl;
    }
    else {
        while (temp.size() != 1 && temp2.size() != 1) {
            for (vector<string>::iterator it = temp.begin(); it != temp.end(); it++) {
                if (((it + 1) == temp.end()) && ((temp.size() % 2) == 1)) { // si es impar y estoy en el ultimo lugar, agrego uno igual al final
                    temp.push_back(*it);
                }
                newID = *it;
                newID += *(it + 1);
                newID = int2hex(generateID(newID.c_str()));
                temp2.push_back(newID);
                fullMerkleTree.push_back(newID);
                it++;
            }
            temp.clear();
            if (temp2.size() == 1) {
                break;
            }
            for (vector<string>::iterator it = temp2.begin(); it != temp2.end(); it++) {
                if (((it + 1) == temp2.end()) && ((temp2.size() % 2) == 1)) { // si es impar y estoy en el ultimo lugar, agrego uno igual al final
                    temp2.push_back(*it);
                }
                newID = *it;
                newID += *(it + 1);
                newID = int2hex(generateID(newID.c_str()));
                temp.push_back(newID);
                fullMerkleTree.push_back(newID);
                it++;
            }
            temp2.clear();
        }
    }

    return fullMerkleTree;
}

// Calcula el merkle root
std::string blockchain::calculatemerkleroot(int num) {
    return calculatemerkletree(num).back();
}
//  SETTERS
// set block id
void blockchain::setblockid(int num, string id) {
    if (num < 0 || num <= cantblocks) {//si esta fuera de rango
        return;
    }
    else {
        j[num].at("blockid") = id;
    }
}
//  set height
void blockchain::setheight(int num, std::string height) {
    if (num < 0 || num <= cantblocks) {//si esta fuera de rango
        return;
    }
    else {
        j[num].at("height") = height;
    }
}
// set merkleroot
void blockchain::setmerkleroot(int num, std::string mk) {
    if (num < 0 || num <= cantblocks) {//si esta fuera de rango
        return;
    }
    else {
        j[num].at("merkleroot") = mk;
    }
}
//  set nTx
void blockchain::setnTx(int num, int nTx) {
    if (num < 0 || num <= cantblocks) {//si esta fuera de rango
        return;
    }
    else {
        j[num].at("nTx") = nTx;
    }
}
//  set nonce
void blockchain::setnonce(int num, int nonce) {
    if (num < 0 || num <= cantblocks) {//si esta fuera de rango
        return;
    }
    else {
        j[num].at("nonce") = nonce;
    }
}
//  set previousblockid
void blockchain::setpreviousblockid(int num, std::string pvid) {
    if (num < 0 || num <= cantblocks) {//si esta fuera de rango
        return;
    }
    else {
        j[num].at("previousblockid") = pvid;
    }
}
//  set ntxin
void blockchain::setnTxin(int block, int tx, int in) {
    if (block < 0 || block <= cantblocks) {//si esta fuera de rango
        return;
    }
    else {
        nlohmann::json bloque = getblock(block);
        int nTx = getnTx(bloque);
        if (tx < 0 || tx <= nTx) {//si esta fuera de rango
            return;
        }
        else {
            j[block].at("tx")[tx].at("nTxin") = in;
        }
    }
}
//  set ntxout
void blockchain::setnTxout(int block, int tx, int out) {
    if (block < 0 || block <= cantblocks) {//si esta fuera de rango
        return;
    }
    else {
        nlohmann::json bloque = getblock(block);
        int nTx = getnTx(bloque);
        if (tx < 0 || tx <= nTx) {//si esta fuera de rango
            return;
        }
        else {
            j[block].at("tx")[tx].at("nTxout") = out;
        }
    }
}
//  set txid
void blockchain::settxid(int block, int tx, string id) {
    if (block < 0 || block <= cantblocks) {//si esta fuera de rango
        return;
    }
    else {
        nlohmann::json bloque = getblock(block);
        int nTx = getnTx(bloque);
        if (tx < 0 || tx <= nTx) {//si esta fuera de rango
            return;
        }
        else {
            j[block].at("tx")[tx].at("txid") = id;
        }
    }
}
//  set outputIndex
void blockchain::setoutputIndex(int block, int tx, int ntxin, int out) {
    if (block < 0 || block <= cantblocks) {//si esta fuera de rango
        return;
    }
    else {
        nlohmann::json bloque = getblock(block);
        int nTx = getnTx(bloque);
        if (tx < 0 || tx <= nTx) {//si esta fuera de rango
            return;
        }
        else {
            nlohmann::json Tx = gettx(bloque, tx);
            int nTxin = getnTxin(Tx);
            if (ntxin < 0 || ntxin <= nTxin) {//si esta fuera de rango
                return;
            }
            else {
                j[block].at("tx")[tx].at("vin")[ntxin].at("outputIndex") = out;
            }
        }
    }
}
//set signature
void blockchain::setsignature(int block, int tx, int ntxin, string signature) {
    if (block < 0 || block <= cantblocks) {//si esta fuera de rango
        return;
    }
    else {
        nlohmann::json bloque = getblock(block);
        int nTx = getnTx(bloque);
        if (tx < 0 || tx <= nTx) {//si esta fuera de rango
            return;
        }
        else {
            nlohmann::json Tx = gettx(bloque, tx);
            int nTxin = getnTxin(Tx);
            if (ntxin < 0 || ntxin <= nTxin) {//si esta fuera de rango
                return;
            }
            else {
                j[block].at("tx")[tx].at("vin")[ntxin].at("signature") = signature;
            }
        }
    }
}
//  set amount
void blockchain::setamount(int block, int tx, int ntxout, int amount) {
    if (block < 0 || block <= cantblocks) {//si esta fuera de rango
        return;
    }
    else {
        nlohmann::json bloque = getblock(block);
        int nTx = getnTx(bloque);
        if (tx < 0 || tx <= nTx) {//si esta fuera de rango
            return;
        }
        else {
            nlohmann::json Tx = gettx(bloque, tx);
            int nTxout = getnTxout(Tx);
            if (ntxout < 0 || ntxout <= nTxout) {//si esta fuera de rango
                return;
            }
            else {
                j[block].at("tx")[tx].at("vout")[ntxout].at("amount") = amount;
            }
        }
    }
}

//  set publicid
void blockchain::setpublicid(int block, int tx, int ntxout, string publicid) {
    if (block < 0 || block <= cantblocks) {//si esta fuera de rango
        return;
    }
    else {
        nlohmann::json bloque = getblock(block);
        int nTx = getnTx(bloque);
        if (tx < 0 || tx <= nTx) {//si esta fuera de rango
            return;
        }
        else {
            nlohmann::json Tx = gettx(bloque, tx);
            int nTxout = getnTxout(Tx);
            if (ntxout < 0 || ntxout <= nTxout) {//si esta fuera de rango
                return;
            }
            else {
                j[block].at("tx")[tx].at("vout")[ntxout].at("publicid") = publicid;
            }
        }
    }
}
//  set vout
void blockchain::setvout(int block, int tx, int amount, string publicid) {
    if (block < 0 || block <= cantblocks) {//si esta fuera de rango
        return;
    }
    else {
        nlohmann::json bloque = getblock(block);
        int nTx = getnTx(bloque);
        if (tx < 0 || tx <= nTx) {//si esta fuera de rango
            return;
        }
        else {//creo un nuevo vout e incremento nTxout
            string aux;
            nlohmann::json bloque, Tx, aux1;
            bloque = getblock(block);//pasar num como si fuese un indice
            Tx = gettx(bloque, tx);//pasar num como indice
            aux = str(boost::format("{\"amount\": %1% ,\"publicid\" : \"%2%\"}") % amount % publicid);
            aux1 = nlohmann::json::parse(aux);
            setnTxout(block, tx, getnTxout(Tx) + 1);
            j[block].at("tx")[tx].at("vout")[getnTxout(Tx)] = aux1;//apunta al nuevo elemento del arreglo
        }
    }
}
//  set vin
void blockchain::setvin(int block, int tx, string blockid, int out, string signature, string txid) {
    if (block < 0 || block <= cantblocks) {//si esta fuera de rango
        return;
    }
    else {
        nlohmann::json bloque = getblock(block);
        int nTx = getnTx(bloque);
        if (tx < 0 || tx <= nTx) {//si esta fuera de rango
            return;
        }
        else {//creo un nuevo vout e incremento nTxout
            string aux;
            nlohmann::json bloque, Tx, aux1;
            bloque = getblock(block);//pasar num como si fuese un indice
            Tx = gettx(bloque, tx);//pasar num como indice
            aux = str(boost::format("{ \"blockid\": \"%1%\",\"outputIndex\" : %2%,\"signature\" : \"%3%\",\"txid\" : \"%4%\" }") % blockid % out % signature % txid);
            aux1 = nlohmann::json::parse(aux);
            setnTxin(block, tx, getnTxin(Tx) + 1);
            j[block].at("tx")[tx].at("vin")[getnTxin(Tx)] = aux1;//apunta al nuevo elemento del arreglo
        }
    }
}
//set tx
void blockchain::settx(int block, string txid) {
    if (block < 0 || block <= cantblocks) {//si esta fuera de rango
        return;
    }
    else {
        string aux;
        nlohmann::json bloque, aux1;
        bloque = getblock(block);//pasar num como si fuese un indice
        aux = str(boost::format("{\"nTxin\": 0,\"nTxout\" : 0,\"txid\" : \"%1%\",\"vin\" : [] ,\"vout\": []}") % txid);
        aux1 = nlohmann::json::parse(aux);
        setnTx(block, getnTx(bloque) + 1);
        j[block].at("tx")[getnTx(bloque)] = aux1;//apunta al nuevo elemento del arreglo
    }
}
// set block
void blockchain::setblock(std::string blockid,  std::string merkleroot, int nonce) {
    string aux;
    nlohmann::json aux1;
    aux = str(boost::format("{ \"blockid\": \"%1%\",\"height\" : %2%,\"merkleroot\" : \"%3%\",\"nTx\" : 0,\"nonce\" : %4%,\"previousblockid\" : \"%5%\",\"tx\" : [] }") % blockid % cantblocks % merkleroot % nonce % getblockid(getblock(cantblocks - 1)));
    aux1 = nlohmann::json::parse(aux);
    j[cantblocks] = aux1;//apunta al nuevo elemento del arreglo
    cantblocks++;
}
