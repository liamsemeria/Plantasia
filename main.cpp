#include<iostream>
#include "Interpreter.h"

int main(int argc, char** argv)
{
    Interpreter p(argv[1]);
    // add inputs
    for (int i = 2; i < argc; i++) {
        if (isdigit(argv[i][0])) p.inputs.push_back(atoi(argv[i]));
        else p.inputs.push_back(argv[i][0]);
    }
    p.lex();
    //p.printCode();
    p.interpret();
    //std::cout << stoi("2") << std::endl;
}
