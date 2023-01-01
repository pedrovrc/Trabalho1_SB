#include <fstream>
#include <regex>
using namespace std;

struct token
{
    string word;
    int linha;
};

struct Simbolo
{
    string word;
    int pos;
    int linha;
};

bool invalido(string token);

void parser();

bool label(string word);

int instrucao(string token);

int diretiva(string token);

void tradutor();
