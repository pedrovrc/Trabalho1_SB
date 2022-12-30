#include <fstream>
#include <iostream>
using namespace std;

#define MACRO_QUANT 4

/*
    Processa macros e salva resultado num arquivo chamado macros.mcr
*/
void macros(string nome_arquivo) {
            // PRIMEIRA PASSADA
            // Procura macros e salva definicoes
    // abre codigo para leitura
    ifstream codigo_preprocessado;
    codigo_preprocessado.open(nome_arquivo);

    string def_macros[MACRO_QUANT];     // 1 string por macro
    string name_macros[MACRO_QUANT];
    int size_macros[MACRO_QUANT];
    string line, aux;
    int count_macros = 0;
    int count_size = 0;

    while(!(codigo_preprocessado.eof())) {
        getline(codigo_preprocessado, line);

        if (line.find("MACRO") != string::npos) {
            count_macros++;

            // salva nome da macro
            aux.clear();
            for (int i = 0; i < (int)line.length(); i++) {
                if (line[i] == ':') {
                    break;
                }
                aux += line[i];
            }
            name_macros[count_macros-1] = aux;

            // salva definicao
            aux.clear();
            getline(codigo_preprocessado, line);
            count_size = 1;
            while(line.find("ENDMACRO") == string::npos) {
                aux += line;
                aux += '\n';
                getline(codigo_preprocessado, line);
                count_size++;
            }
            def_macros[count_macros-1] = aux;
            size_macros[count_macros-1] = count_size;
        }
    }
    codigo_preprocessado.close();


            // SEGUNDA PASSADA
            // Inclui macros no codigo
    // abre codigo para leitura
    codigo_preprocessado.open(nome_arquivo);

    // cria arquivo para escrita
    ofstream macros_processadas;
    macros_processadas.open("macros.mcr");

    int call_pos;
    bool flag_macro_called = false;

    while(!(codigo_preprocessado.eof())) {
        getline(codigo_preprocessado, line);

        // pula linha com "ENDMACRO"
        if (line.find("ENDMACRO") != string::npos) {
            continue;
        }

        flag_macro_called = false;
        for (int i = 0; i < MACRO_QUANT; i++) {
            // busca chamada de macros conhecidas
            if (line.find(name_macros[i]) != string::npos && name_macros[i].length() > 0) {
                call_pos = line.find(name_macros[i]);

                if (line[call_pos + name_macros[i].length()] == ':') {
                    // encontra ':' apos nome da macro -> declaracao
                    // pula toda a declaracao
                    for (int j = 0; j < size_macros[i] + 1; j++) {
                        getline(codigo_preprocessado, line);
                    }
                    continue;
                } else {
                    // sem ':' apos nome da macro -> chamada
                    // inclui a definicao no codigo
                    macros_processadas << def_macros[i];
                    flag_macro_called = true;
                    continue;
                }
            }
        }

        if (flag_macro_called == false) {
            line += '\n';
            macros_processadas << line;
        }
    }

    macros_processadas.close();
    codigo_preprocessado.close();
    
    return;
}
