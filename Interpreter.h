#include <iostream>
#include <sstream>
#include <fstream>
#include <stack>
#include <vector>
#include <map>
// node to construct flow graph
struct node
{
    std::vector<std::string> expression;
    node* next = NULL;
    node* jump = NULL;
};
// parse the file, while adding new variables and stuff
class Interpreter
{
    // first node of the flow graph
    node* head;

    // gardens are stacks of variables
    std::stack<int> garden;

    // map for variables
    std::map<std::string, int> variables;
    
    // map for functions
    std::map<std::string, node*> functions;

    // source code file
    std::string fileName;
    public:

    Interpreter(std::string file);

    // convert file imto nodes
    void lex();
    // convert tokens into a FlowGraph
    // interpret and execute the code
    void interpret();

    void printCode();
    private:
    // helper functions
    //void printCode();
    // add or subtract plant to garden and push the value
    void pushChangedValue(std::string s, char opp);

};
