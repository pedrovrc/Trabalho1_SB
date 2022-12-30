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

    Arquivo sem extensao - assumir que usuario usa extensoes corretas
    Operações possíveis: -p, -m, -o.
*/

int main(int argc, char* argv[]) {
    if (argc == 3) {    // qtd de argumentos correta

        string nome_arquivo;
        nome_arquivo = (string)argv[2] + ".asm";

        // converte texto para all caps
        string nome_arq_upper;
        nome_arq_upper = (string)argv[2] + "UPPER.asm";
        file2UPPER(nome_arquivo, nome_arq_upper);
        
        string nome_preproc;
        nome_preproc = "preproc.pre";

        if ((string)argv[1] == "-p") {  // executa pre-processamento
            preprocessamento(nome_arq_upper);

        } else if ((string)argv[1] == "-m") {   // executa macros
            preprocessamento(nome_arq_upper);
            macros(nome_preproc);

        } else if ((string)argv[1] == "-o") {   // executa pre-processamento, macros e tradutor
            preprocessamento(nome_arq_upper);
            macros(nome_preproc);

            fstream codigo_macros;
            codigo_macros.open("macros.mcr", ios::in);
            
            tradutor(codigo_macros);

            codigo_macros.close();
        } else {    // diretiva de uso do programa nao reconhecida
            cout << "Uso incorreto do programa1. Exemplo de uso: ./montador -o programa" << endl;
        }

    } else {    // qtd de argumentos incorreta
        cout << "Uso incorreto do programa2. Exemplo de uso: ./montador -o programa" << endl;
    }

    return 0;
}