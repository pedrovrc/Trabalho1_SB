#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "preprocessamento.h"
#include "macros.h"
#include "tradutor.h"

/*
    Modo de usar o programa:
    ./montador -<op> -<arquivo>

    Arquivo sem extensão - assumir que usuário usa extensões corretas
    Operações possíveis: -p, -m, -o.
*/

int main(int argc, char* argv[]) {
    if (argc == 3) {    // qtd de argumentos correta

        fstream codigo_base;
        string nome_arquivo;
        nome_arquivo = (string)argv[2] + ".asm";
        codigo_base.open(nome_arquivo, ios::in);

        if (!codigo_base) {
            cout << "Erro ao abrir arquivo." << endl;
            return 0;
        }

        if ((string)argv[1] == "-p") {  // executa pre-processamento
            preprocessamento(codigo_base);
        } else if ((string)argv[1] == "-m") {   // executa macros
            preprocessamento(codigo_base);

            fstream codigo_preprocessado;
            codigo_preprocessado.open("preproc.pre", ios::in);

            macros(codigo_preprocessado);

            codigo_preprocessado.close();
        } else if ((string)argv[1] == "-o") {   // executa pre-processamento, macros e tradutor
            preprocessamento(codigo_base);
            
            fstream codigo_preprocessado;
            codigo_preprocessado.open("preproc.pre", ios::in);

            macros(codigo_preprocessado);

            fstream codigo_macros;
            codigo_macros.open("macros.mcr", ios::in);
            
            tradutor(codigo_macros);

            codigo_preprocessado.close();
            codigo_macros.close();
        } else {    // diretiva de uso do programa nao reconhecida
            cout << "Uso incorreto do programa1. Exemplo de uso: ./montador -o programa" << endl;
        }

        codigo_base.close();

    } else {    // qtd de argumentos incorreta
        cout << "Uso incorreto do programa2. Exemplo de uso: ./montador -o programa" << endl;
    }

    return 0;
}