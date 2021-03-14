#include <iostream>
#include <sstream>
#include <fstream>
#include <stack>
#include <vector>
#include <map>
// parse the file, while adding new variables and shit
class Interpreter
{
    // gardens are stacks of variables
    std::stack<int> garden;
    // all of the code from file put int 2d vector
    std::vector<std::vector<std::string> > code;

    // map for variables
    std::map<std::string, int> variables;

    // source code file
    std::string fileName;
    public:

    Interpreter(std::string file);

    // parse the file
    void parse();
    // interpret and execute the code
    void interpret();

    void printCode();
    private:
    // helper functions
    //void printCode();
    // add or subtract plant to garden and push the value
    void pushChangedValue(std::string s, int sign);

};