#include<iostream>
#include "Interpreter.h"

int main()
{
    Interpreter p("source.txt");
    p.parse();
    p.interpret();
}