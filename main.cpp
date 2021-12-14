#include <bits/stdc++.h>
#include "helper_functions.h"
#include "interpreter.h"
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
    string contents;
    string temp;
    while (getline(file, temp))
        contents+=temp+" ";
    I.interpret(contents);
}