// tp final.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include"Header.h"
#include"blockchain.h"

using namespace std;

int main()
{
    int num =0;
    string aux;
    nlohmann::json bloque;
    std::string file = "ejemplo.json";
    blockchain prueba(file);
    bloque=prueba.getblock(num);//pasar indice como si fuese un array
    cout << "id del bloque: " << prueba.getblockid(bloque) << endl;
    cout <<"id del bloque anterior: "<< prueba.getpreviousblockid(bloque) << endl;
    cout <<"height: "<< prueba.getheight(bloque) << endl;
    cout <<"merkleroot: "<< prueba.getmerkleroot(bloque) << endl;
    cout<<"nonce: "<< prueba.getnonce(bloque) << endl;
    cout<<"numero de transacciones: "<< prueba.getnTx(bloque) << endl;
}
