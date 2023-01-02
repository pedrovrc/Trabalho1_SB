#include <fstream>
#include <iostream>
#include "tradutor.h"
using namespace std;

bool invalido(string token)
{
    if (regex_match(token, regex("^-\\d+$")))
        return false;
    if (regex_match(token, regex("^\\d+\\D+")))
        return true;
    if (regex_match(token, regex("^\\W+\\w+")))
        return true;
    return false;
}

void parser()
{
    fstream codigo_base;

    codigo_base.open("macros.mcr", fstream::in);

    token temp;
    vector<token> words;
    string word, line;
    int linha = 1;

    while (getline(codigo_base, line))
    {
        istringstream iss(line);
        while (iss >> word)
        {
            temp.linha = linha;
            temp.word = word;
            words.push_back(temp);
        }
        linha++;
    }

    for (int i = 0; i < (int)words.size(); i++)
        if (invalido(words[i].word))
            cout << "Erro lexico na linha " << words[i].linha << " :" << words[i].word << endl;

    codigo_base.close();
}

bool label(string word)
{
    if (regex_match(word, regex("\\w+:$")))
        return true;
    return false;
}

int instrucao(string token)
{
    if (token == "ADD")
        return 1;
    else if (token == "SUB")
        return 2;
    else if (token == "MUL")
        return 3;
    else if (token == "DIV")
        return 4;
    else if (token == "JMP")
        return 5;
    else if (token == "JMPN")
        return 6;
    else if (token == "JMPP")
        return 7;
    else if (token == "JMPZ")
        return 8;
    else if (token == "COPY")
        return 9;
    else if (token == "LOAD")
        return 10;
    else if (token == "STORE")
        return 11;
    else if (token == "INPUT")
        return 12;
    else if (token == "OUTPUT")
        return 13;
    else if (token == "STOP")
        return 14;
    return 0;
}

int diretiva(string token)
{
    if (token == "SPACE")
        return 1;
    else if (token == "CONST")
        return 2;
    else if (token == "SECTION")
        return 3;
    else if (token == "DATA")
        return 4;
    else if (token == "TEXT")
        return 5;
    return 0;
}

void tradutor()
{
    fstream codigo_base;

    codigo_base.open("macros.mcr", fstream::in);

    Simbolo temp;
    vector<Simbolo> words;
    string word, line;
    int linha = 1;
    int pos = 0;
    bool lastWordSpace = false;

    while (getline(codigo_base, line))
    {
        istringstream iss(line);
        while (iss >> word)
        {
            if (label(word))
            {
                // temp.word = word;
                temp.word = regex_replace(word, regex(":"), "$2");
                temp.linha = linha;
                temp.pos = pos;
                words.push_back(temp);
                pos--;
            }
            else if (regex_match(word, regex("^SPACE$")))
            {
                lastWordSpace = true;
            }
            else if (regex_match(word, regex("^CONST$")))
                pos--;
            else if (regex_match(word, regex("\\w+,\\w+"))) // caso sejam duas variaveis
                pos++;
            // else if (regex_match(word, regex("[A-Z]+\\+\\d+"))) // CASO X+2
            else if (lastWordSpace && !regex_match(word, regex("^\\d+")))
                lastWordSpace = false;
            else if (lastWordSpace && regex_match(word, regex("^\\d+"))) // CASO SPACE 10
                pos += (stoi(word)) - 1;
            pos++;
        }
        linha++;
    }
    // aqui temos a tabela de simbolos completa

    // reset stream
    codigo_base.close();
    codigo_base.open("macros.mcr", fstream::in);

    vector<int> resultado;
    lastWordSpace = false;
    bool lastWordSConst = false;
    bool lastWordLabel = false;
    linha = 0;

    while (getline(codigo_base, line))
    {
        linha++;
        istringstream iss(line);
        while (iss >> word)
        {
            if ((lastWordSConst || lastWordSpace) && !regex_match(word, regex("^\\d+$")))
            {
                lastWordSConst = false;
                lastWordSpace = false;
            }

            if (lastWordLabel && !label(word))
            {
                lastWordLabel = false;
            }
            
            if (instrucao(word))
            {
                int temp = instrucao(word);
                resultado.push_back(temp);
                
            }
            else if (diretiva(word))
            {
                if (diretiva(word) == 1) // SPACE
                {
                    lastWordSpace = true;
                    resultado.push_back(0);
                }
                else if (diretiva(word) == 2)
                {
                    lastWordSConst = true;
                }
            }
            else if (label(word))
            {
                if (lastWordLabel)
                    cout << "Erro semantico na linha " << linha << ": Mais de um label na linha" << endl;
                lastWordLabel = true;
            }
            else if (regex_match(word, regex("^\\d+$"))) // numero
            {
                if (lastWordSConst)
                {
                    // escreve o numero word
                    resultado.push_back(stoi(word));
                }
                else if (lastWordSpace)
                {
                    resultado.pop_back();
                    // escreve 0 word vezes
                    for (int i = 0; i < stoi(word); i++)
                    {
                        resultado.push_back(0);
                    }
                }
            }

            else // variavel
            {
                if (regex_match(word, regex("^\\w+\\+\\d+$"))) // caso L1+10
                {
                    // escreve words[word].pos+n
                    string temp1 = regex_replace(word, regex("\\+\\d+$"), "$2");
                    int temp2 = stoi(regex_replace(word, regex("^\\w+\\+"), "$2"));

                    bool encontrado = false;
                    for (int i = 0; i < (int)words.size(); i++)
                    {
                        if (words[i].word == temp1)
                        {
                            resultado.push_back(words[i].pos + temp2);
                            encontrado = true;
                        }
                    }
                    if (!encontrado)
                    {
                        // erro de label nao declarado
                        cout << "Erro semantico na linha " << linha << ": Rotulo nao definido" << endl;
                    }
                }
                else if (regex_match(word, regex("^\\w+\\,\\w+$"))) // caso L1,L2
                {
                    // escreve o l1.pos da tabela de simbolos e l2.pos da tabela de simbolos
                    string temp1 = regex_replace(word, regex(",\\w+$"), "$2");
                    string temp2 = regex_replace(word, regex(",\\w+$"), "$2");

                    bool encontrado = false;
                    for (int i = 0; i < (int)words.size(); i++)
                    {
                        if (words[i].word == temp1)
                        {
                            resultado.push_back(words[i].pos);
                            encontrado = true;
                        }
                    }
                    if (!encontrado)
                    {
                        // erro de label nao declarado
                        cout << "Erro semantico na linha " << linha << endl;
                    }

                    encontrado = false;
                    for (int i = 0; i < (int)words.size(); i++)
                    {
                        if (words[i].word == temp2)
                        {
                            resultado.push_back(words[i].pos);
                            encontrado = true;
                        }
                    }
                    if (!encontrado)
                    {
                        // erro de label nao declarado
                        cout << "Erro semantico na linha " << linha << endl;
                    }
                    else
                    {
                            cout << "Erro semantico na linha " << linha << ": numero errado de argumentos" << endl;
                    }
                    
                }
                else // variavel padrao
                {
                    bool encontrado = false;
                    for (int i = 0; i < (int)words.size(); i++)
                    {
                        if (words[i].word == word)
                        {
                            resultado.push_back(words[i].pos);
                            encontrado = true;
                        }
                    }
                    if (!encontrado)
                    {
                        // erro de label nao declarado
                        cout << "Erro semantico na linha " << linha << endl;
                    }
                }
            }
        }
    }

    ofstream saida;
    saida.open("saida.o");

    for (int i = 0; i < (int)resultado.size(); i++)
    {

        cout << resultado[i] << " ";
        saida << resultado[i] << " ";
    }
    cout << endl;

    codigo_base.close();
}

/*
    Processa tradução e salva resultado num arquivo .o
*/
