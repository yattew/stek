#include "includes.h"
using namespace std;

int main(int argc, char *argv[])
{
    Interpreter I;
    if ( argc == 1)
    {
        while (true)
        {
            string in;
            cout << ">> ";
            getline(cin, in, '\n');
            I.interpret(in);
        }
    }
    char*name = *(++argv);
    ifstream file(name);
    string temp;
    string all;
    while (getline(file,temp))
        I.interpret(temp);
}