#include <bits/stdc++.h>
#include "helper_functions.h"
#include "interpreter.h"
using namespace std;

int main(int argc, char *argv[])
{
    Interpreter I;
    if (argc == 1)
    {
        while (true)
        {
            string in;
            cout << ">> ";
            getline(cin, in, '\n');
            I.interpret(in);
        }
    }
    else if (argc == 2)
    {
        string name = "";
        int idx = 0;
        while(argv[1][idx]!='\0')
        {
            name.push_back(argv[1][idx]);
            idx++;
        }
        ifstream file(name);
        string contents;
        string temp;
        while(getline(file,temp))
        {
            contents.append(temp);
            contents.push_back(' ');
        }
        I.interpret(contents);
    }
}