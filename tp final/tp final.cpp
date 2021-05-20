// tp final.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include"Header.h"
#include"blockchain.h"

using namespace std;

int main()
{
    int num,numoutofrange;
    num = 0;
    numoutofrange= 10;
    string aux1,aux2,aux3,aux4;
    char buffer[100] = { 0 };
    nlohmann::json bloque,tx,vin,vout;
    std::string file = "ejemplo.json";
    blockchain prueba(file);
    bloque=prueba.getblock(num);//pasar num como si fuese un indice
    tx= prueba.gettx(bloque, 0);
    tx = prueba.gettx(bloque,num);//pasar num como indice
    vin = prueba.getvin(tx, num);//pasar num como indice
    vout = prueba.getvout(tx, num);//pasar num como indice

    cout << "id del bloque: " << prueba.getblockid(bloque) << endl;
    cout <<"id del bloque anterior: "<< prueba.getpreviousblockid(bloque) << endl;
    cout <<"height: "<< prueba.getheight(bloque) << endl;
    cout <<"merkleroot: "<< prueba.getmerkleroot(bloque) << endl;
    cout<<"nonce: "<< prueba.getnonce(bloque) << endl;
    cout<<"numero de transacciones: "<< prueba.getnTx(bloque) << endl;
    cout << "id de la  transaccion 1: " << prueba.gettxid(tx) << endl;
    cout << "cant de vin en transaccion 1: " << prueba.getnTxin(tx) << endl;
    cout << "cant de vout en transaccion 1: " << prueba.getnTxout(tx) << endl;
    cout << "en vin 1: " << endl;
    cout << "block id: " << prueba.getblockid(vin) << endl;
    cout << "tx id: " << prueba.gettxid(vin) << endl;
    cout << "outputindex: " << prueba.getoutputIndex(vin) << endl;
    cout<<"signature: "<< prueba.getsignature(vin) << endl;
    cout << "en vout 1: " << endl;
    cout << "amount: " << prueba.getamount(vout) << endl;
    cout<<"publicid: "<< prueba.getpublicid(vout) << endl;
    
    
}
