#include "includes.h"
using namespace std;

int main(int argc, char *argv[])
{
    Interpreter I;
    Lexer L;
    if (argc == 1)
    {
        while (true)
        {
            string in;
            cout << ">> ";
            getline(cin, in, '\n');
            vector<Object> objects = L.tokenize(in);
            I.interpret(objects);
        }
    }
    char *name = *(++argv);
    ifstream file(name);
    string temp;
    string all;
    while (getline(file, temp))
    {
        vector<Object> objects = L.tokenize(temp);
        I.interpret(objects);
    }
}