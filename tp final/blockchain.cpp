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
std::string blockchain::calculatemerkleroot(int num)
{
    int itx = 0;
    int itxin = 0;
    string aux = "";
    string merkleroot="";
    list<string> hojas;
    nlohmann::json bloque,tx,vin;
    if (getblock(num) == nulljson) {//ese bloque no existe
        return merkleroot;
    }
    bloque = getblock(num);
    itx = getnTx(bloque);
    for (int i = 0; i < itx; ++i) {//creo un lista con las hojas
        tx = gettx(bloque,i);
        itxin = getnTxin(tx);
        for (int j = 0; j < itxin; j++) {
            vin = getvin(tx, j);
            aux += gettxid(vin);
        }
        aux = int2hex(generateID(aux.c_str()));
        aux.resize(8);
        aux.shrink_to_fit();
        hojas.push_back(aux);
        if (((itx%2)!=0)&& (i == itx - 1)) {//si estoy en la ultima hoja, y esta es impar, la vuelvo a agregar a la lista
            hojas.push_back(aux);
        }
        aux = "";
    }
    cout << "tx ids, osea hojas: " << endl;
    for (auto n : hojas) {
        static int k= 1;
        cout << "hoja"<<k<<": "<<n.substr()<<endl;
        k++;
    }
    //
    //
    bool par = false;
    for (std::list<string>::iterator it = hojas.begin(); it != hojas.end(); ++it) {
        if (par == false) {//si me falta concatenar un elemento
            aux += *it;
            par = true;
        }
        else {
            aux += *it;
            aux = int2hex(generateID(aux.c_str()));
            cout << aux << endl;
            //aca lo guardas en algun lado
            aux = "";
            par = false;
        }
    }//repetir el for pero con los nodos ya calculados, idealmente hacer de este for una funcion o meterlo dentro de otro for
    //antes de repetir verificar paridad
    //meter datos en un tree
    aux = "7CD22096";//esto despues lo sacas
    aux += "24387EF4";//esto despues lo sacas
    aux = int2hex(generateID(aux.c_str()));
    cout << aux << endl;
    return merkleroot;
}
// 
