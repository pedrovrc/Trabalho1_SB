#include <fstream>
#include <iostream>
using namespace std;

/*
    Processa macros e salva resultado num arquivo chamado macros.mcr
*/
void macros(string nome_arquivo) {
    // abre codigo para leitura
    ifstream codigo_preprocessado;
    codigo_preprocessado.open(nome_arquivo);

    // cria arquivo para escrita
    ofstream macros_processadas;
    macros_processadas.open("macros.mcr");


    macros_processadas.close();
    codigo_preprocessado.close();
    
    return;
}
