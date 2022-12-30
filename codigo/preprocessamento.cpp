#include <fstream>
#include <iostream>
using namespace std;

/*
    Processa diretivas EQU e IF e salva resultado num arquivo chamado preproc.pre
*/
void preprocessamento(string nome_arquivo) {
    ifstream codigo_base;
    codigo_base.open(nome_arquivo);

        if (!(codigo_base.good())) {
            cout << "Erro ao abrir arquivo." << endl;
            return;
        }

    // abre novo arquivo para escrita
    ofstream newcode;
    newcode.open("preproc.pre");
    if (!(newcode.good())) {
        cout << "Erro ao criar arquivo" << endl;
        return;
    }

    string line;
    string var_name;
    string var_value;
    string names[5];
    string values[5];
    char aux;
    int i, pos;
    int count_EQU = 0;

    // loop para leitura e escrita
    while (!(codigo_base.eof())) {
        getline(codigo_base, line);

        // retira comentario
        if (line.find(";") != string::npos) {
            // ignora espacos
            i = 0;
            while(isspace(line[i])) {
                i++;
            }
            if (line[i] == ';') {   // comentario de linha inteira
                continue;

            } else {                // comentario inline
                i = 0;
                while(line[i] != ';') {
                    i++;
                }
                if (line[i] == ';') {
                    line.erase(i, string::npos);
                }
            }
        }

        if (line.find("EQU") != string::npos) {
            // processa EQU
            // obtem nome da variavel do EQU
            var_name.clear();
            i = 0;
            aux = '0';
            while (!(isspace(line[i]))) {
                aux = line[i];
                var_name += aux;
                i++;
            }

            // obtem valor a ser substituido
            var_value.clear();
            i = line.find("EQU") + 3;
            aux = '0';
            // pula espacos
            while ((isspace(line[i]))) {
                i++;
            }
            // armazena valor
            while (!(isspace(line[i])) && i < (int)line.length()) {
                aux = line[i];
                var_value += aux;
                i++;
            }

            count_EQU++;

            // salva nome e valor
            names[count_EQU-1] = var_name;
            values[count_EQU-1] = var_value;

            continue;
        }

        // modificacoes para EQU
        if (count_EQU > 0) {
            for (int i = 0; i < count_EQU; i++) {
                if (line.find(names[i]) != string::npos) {
                    pos = line.find(names[i]);
                    line.replace(pos, names[i].length(), values[i]);
                }
            }
        }

        if (line.find("IF") != string::npos) {
            // processa IF
            i = 2;
            while (isspace(line[i])) {
                i++;
            }

            if (line[i] == '0') {
                getline(codigo_base, line);
            }
            continue;
        }

        // inclui linha no novo arquivo e limpa string
        line.append("\n");
        newcode << line;
        line.clear();
    }

    newcode.close();
    codigo_base.close();

    return;
}
