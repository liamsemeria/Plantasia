#include <iostream>
#include <sstream>
#include <fstream>
#include <stack>
#include <map>
// parse the file, while adding new variables and shit
class Interpreter
{
    // gardens are stacks of variables
    std::stack<int> garden;
    // stack of tokens to evaluate a line
    std::stack<std::string> line;
    // map for variables
    std::map<std::string, int> variables;

    // source code file
    std::string fileName;
    public:
    Interpreter(std::string file);

    // parse the file
    void interpret();
    // helper functions

    void gardenToString();
    // add or subtract plant to garden and push the value
    void pushChangedValue(std::string s, int sign);

};