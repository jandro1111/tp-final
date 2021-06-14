#include "blockchain.h"
using namespace std;
//	generate id	//
std::string generateID(std::string message)
{
    /*unsigned int ID = 0;
    int c;
    while (c = *str++)
    {
        ID = c + (ID << 6) + (ID << 16) - ID;
    }
    return ID;*/
    CryptoPP::SHA256 hash;
    CryptoPP::byte digest[CryptoPP::SHA256::DIGESTSIZE];
    hash.CalculateDigest(digest, (CryptoPP::byte*)message.c_str(), message.length());

    CryptoPP::HexEncoder encoder;
    std::string output;
    encoder.Attach(new CryptoPP::StringSink(output));
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();
    return output;
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


//  calcula el merkle tree
vector<vector<string>> blockchain::calculatemerkletree(int num)
{
    int itx = 0;
    int itxin = 0;
    string aux = "";

    vector<string> temp;
    vector<string> temp2;
    vector<vector<string>> fullMerkleTree; //Contiene todo el merkle tree, empezando por hojas y terminando por el root

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
        aux = generateID(aux);

        /*aux.resize(8);
        aux.shrink_to_fit();*/
        hojas.push_back(aux);
        if (((itx % 2) != 0) && (i == itx - 1)) {//si estoy en la ultima hoja, y esta es impar, la vuelvo a agregar a la lista
            hojas.push_back(aux);
        }
        aux = "";
    }

    // hasta aca calcula las hojas

    for (std::list<string>::iterator it = hojas.begin(); it != hojas.end(); ++it) {
        temp.push_back(*it);
    }
    fullMerkleTree.push_back(temp);

    string newID;
    if (temp.size() == 1) {
        cout << "Block only has one trsnaction!" << endl;
    }
    else {
        while (temp.size() != 1 && temp2.size() != 1) {
            for (vector<string>::iterator it = temp.begin(); it != temp.end(); it++) {
                newID = *it;
                newID += *(it + 1);
                newID = generateID(newID);
                temp2.push_back(newID);
                it++;
            }
            if (temp2.size() == 1) {
                break;
            }
            if (((temp2.size() % 2) == 1)) { // si es impar y estoy en el ultimo lugar, agrego uno igual al final
                temp2.push_back(*(temp2.end() - 1));
            }
            temp.clear();
            fullMerkleTree.push_back(temp2);
            if (temp2.size() == 1) {
                break;
            }
            for (vector<string>::iterator it = temp2.begin(); it != temp2.end(); it++) {
                newID = *it;
                newID += *(it + 1);
                newID = generateID(newID);
                temp.push_back(newID);
                it++;
            }
            if (temp.size() == 1) {
                break;
            }
            if (((temp.size() % 2) == 1)) { // si es impar y estoy en el ultimo lugar, agrego uno igual al final
                temp.push_back(*(temp.end() - 1));
            }
            temp2.clear();
            fullMerkleTree.push_back(temp);
        }
    }

    return fullMerkleTree;
}


// Calcula el merkle root
std::string blockchain::calculatemerkleroot(int num) {
    return calculatemerkletree(num).back().back();
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
//
std::string blockchain::getblocks(int cant,int indi) {
    std::string aux = "";
    for (int i = indi; i < cantblocks&&cant>0; ++i,cant--) {
        aux += getblock(i).dump();
    }
    return aux;
}
//
int blockchain::searchid(std::string id) {
    int i;
    if (id == "00000000") {
        return 0;
    }
    for ( i = 0; i < cantblocks; ++i) {
        if (id == getblockid(getblock(i))){
            break;
        }
    }
    if (i == cantblocks) {//si sali del for pq me quede sin bloques para buscar
        return -1;
    }
    else {//si sali del for pq encontre el id que buscaba
        return i ;
    }
}
//
std::string blockchain::getheader(int cant, int indi) {
    std::string aux = "";
    for (int i = indi+1; i < cantblocks && cant>0; ++i, cant--) {
        aux += makeheader(i).dump();
    }
    return aux;
}
//
nlohmann::json blockchain::makeheader(int i) {
    nlohmann::json header;
    std::string aux;
    aux = str(boost::format("{ \"blockid\": \"%1%\",\"height\" : %2%,\"merkleroot\" : \"%3%\",\"nTx\" : %4%,\"nonce\" : %5%,\"previousblockid\" : \"%6%\" }") % getblockid(getblock(i)) % getheight(getblock(i)) % getmerkleroot(getblock(i)) % getnTx(getblock(i)) % getnonce(getblock(i)) % getpreviousblockid(getblock(i))  );
    header = nlohmann::json::parse(aux);
    return header;
}
//
std::string blockchain::makemerkleblock(int block,int tx) {
    std::string aux = "";
    if (block >= cantblocks) {//si quiero calcular el merklepath de algo q no existe
        return aux;
    }
    if (tx >= getnTx(getblock(block))) {//si quiero calcular el merklepath de algo q no existe
        return aux;
    }
    aux = str(boost::format("{\"blockid\": \"%1%\", \"tx\": %2%,\"txPos\": %3% ,\"merklePath\": [%4%]}") %getblockid(getblock(block)) %gettx(getblock(block),tx) %tx %makemerklepath(block,tx));
    return aux;
}
//
std::string blockchain::makemerklepath(int block, int tx) {
    std::string aux = "";
    list<string> toparse;
    int canth = getnTx(getblock(block));
    vector<vector<std::string>>merkletree = calculatemerkletree(block);
    if (tx % 2 != 0) {//si la tx es impar
        toparse.push_back(merkletree[0][tx - 1]);
    }
    else {
        toparse.push_back(merkletree[0][tx + 1]);
    }
    for (int i = 1; i <(merkletree.capacity()-1);++i) {//itero para el resto de los niveles del arbol salvo en el root
        int j = merkletree[i].capacity();
        if ((tx/(2*i)) % 2 != 0) {//si  es impar
            toparse.push_front(merkletree[i][(tx / (2 * i)) - 1]);
        }
        else {
            toparse.push_back(merkletree[i][(tx / (2 * i)) + 1]);
        }
    }
    int i = 0;
    for (list<string>::iterator it = toparse.begin(); i < toparse.size();++i,it++) {
        if (i == toparse.size()) {//si es el ultimo elemento no le pongo la coma
            aux += str(boost::format("\"id\":\"%1%\"") % *it);
        }
        else {
            aux += str(boost::format("\"id\":\"%1%\",")%*it);
        }
    }
    //cout<<aux<<endl;
    return aux;
}
//
std::string blockchain::sendtx(int amount, std::string publicid) {
    std::string aux = "";
    aux = str(boost::format("{\"nTxin\": 0,\"nTxout\" : 1,\"txid\" : \"algo\",\"vin\" : [] ,\"vout\": [{\"amount\":%1%,\"publicid\":\"%2%\"}]}") % amount %publicid);
    return aux;
}
//
std::string hasheo(std::string m) {
    std::string bin;
    std::string a = generateID(m);
    for (unsigned i = 0; i != a.length(); ++i)
        bin += hex_char_to_bin(a[i]);
    return bin;
}
//
const char* hex_char_to_bin(char c)
{
    // TODO handle default / error
    switch (toupper(c))
    {
    case '0': return "0000";
    case '1': return "0001";
    case '2': return "0010";
    case '3': return "0011";
    case '4': return "0100";
    case '5': return "0101";
    case '6': return "0110";
    case '7': return "0111";
    case '8': return "1000";
    case '9': return "1001";
    case 'A': return "1010";
    case 'B': return "1011";
    case 'C': return "1100";
    case 'D': return "1101";
    case 'E': return "1110";
    case 'F': return "1111";
    }
}
//
nlohmann::json blockchain::mine(std::string tx, int ntx) {
    bool mino = true;
    int i = 0;
    std::string hash;
    string aux;
    nlohmann::json bloque, tohash;
    aux = str(boost::format("{ \"blockid\": \"\",\"height\" : %1%,\"merkleroot\" : \"\",\"nTx\" : %2%,\"nonce\" : 0,\"previousblockid\" : \"%3%\", ") % cantblocks % ntx % getblockid(getblock(cantblocks - 1)));
    aux += tx;
    aux += "}";
    bloque = nlohmann::json::parse(aux);
    cantblocks++;
    j[cantblocks - 1] = bloque;
    bloque.at("merkleroot") = calculatemerkleroot(cantblocks - 1);
    tohash = makeheader(cantblocks - 1);
    //seteo un nonce random
    do {
        mino = true;
        int j = 0;
        cout << "intento numero: " << i << endl;
        //bloque.at("nonce") = i;
        ++i;
        tohash.at("nonce") = (rand() % 65536 + 0);
        hash = hasheo(tohash.dump());
        std::cout << hash << std::endl;
        for (std::string::iterator it = hash.begin(); j < cant0; ++it, ++j) {
            if (*it == '1') {//no cumple con el challange
                mino = false;
                break;
            }
        }

    } while (mino == false);
    bloque.at("nonce") = tohash.at("nonce");
    bloque.at("blockid") = GetHexFromBin(hash);//el bloque id es su hash
    j[cantblocks - 1] = bloque;//lo agrego a la chain

    return bloque;
}

string GetHexFromBin(string sBinary)
{
    string rest(""), tmp, chr = "0000";
    int len = sBinary.length() / 4;
    chr = chr.substr(0, len);
    sBinary = sBinary;
    for (int i = 0; i < sBinary.length(); i += 4)
    {
        tmp = sBinary.substr(i, 4);
        if (!tmp.compare("0000"))
        {
            rest = rest + "0";
        }
        else if (!tmp.compare("0001"))
        {
            rest = rest + "1";
        }
        else if (!tmp.compare("0010"))
        {
            rest = rest + "2";
        }
        else if (!tmp.compare("0011"))
        {
            rest = rest + "3";
        }
        else if (!tmp.compare("0100"))
        {
            rest = rest + "4";
        }
        else if (!tmp.compare("0101"))
        {
            rest = rest + "5";
        }
        else if (!tmp.compare("0110"))
        {
            rest = rest + "6";
        }
        else if (!tmp.compare("0111"))
        {
            rest = rest + "7";
        }
        else if (!tmp.compare("1000"))
        {
            rest = rest + "8";
        }
        else if (!tmp.compare("1001"))
        {
            rest = rest + "9";
        }
        else if (!tmp.compare("1010"))
        {
            rest = rest + "A";
        }
        else if (!tmp.compare("1011"))
        {
            rest = rest + "B";
        }
        else if (!tmp.compare("1100"))
        {
            rest = rest + "C";
        }
        else if (!tmp.compare("1101"))
        {
            rest = rest + "D";
        }
        else if (!tmp.compare("1110"))
        {
            rest = rest + "E";
        }
        else if (!tmp.compare("1111"))
        {
            rest = rest + "F";
        }
        else
        {
            continue;
        }
    }
    return rest;
}
//
void blockchain::setchallenge(int c) {
    cant0 = c;
}