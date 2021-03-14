#include<iostream>
#include "Interpreter.h"

int main(int argc, char** argv)
{
    Interpreter p(argv[1]);
    p.parse();
    //p.printCode();
    std::string s = "asd";
    //if (true && (s[-1]=='c')) std:: cout << "asdasd";
    p.interpret();
}