#include "Interpreter.h"

Interpreter::Interpreter(std::string file)
{
    fileName = file;
    head = new node();
}

// parse the file
// convert line string into array of tokens and construct nodes
// wire up the nodes
void Interpreter::lex()
{
    std::ifstream iFile;
    bool hasTab = false;
    int tabs = 0;
    iFile.open(fileName);
    std::string line;
    std::string token;
    std::vector<std::string> parsedLine;
    // stack to count loop layers
    std::stack<node*> withers;
    int k = 0;
    node* prev = NULL;
    while(getline(iFile,line))
    {
        //std::stringstream s(line);
        // for indent (get rid of beginning spaces)
        for (int i = 0; i < line.length(); i++)
        {
            if (line[i] != ' ')
            {
                line = line.substr(i);
                tabs = i/4;
                hasTab = true;
                break;
            }
        }
        if (hasTab)
        {
            // add correct amount of tabs
            //for (int i = 0; i < tabs; i++)
            //    parsedLine.push_back("TAB");
            tabs = 0;
            hasTab = false;
        }
        // for code
        for (auto &ch : line)
        {
            if (ch == ' ')
            {
                parsedLine.push_back(token);
                //std::cout << token << std::endl;
                token = "";
            } else 
            {
                token+=ch;
            }
        }
        parsedLine.push_back(token);
        //std::cout << token << std::endl;
        token = "";
        if (!k)
        {
            head->expression = parsedLine;
            prev = head;
        }
        else 
        {
            
            //make nodes and wire them to the previous line
            prev->next = new node();
            prev->next->expression = parsedLine;
            // wither end statement
            if (prev->expression[0] == "end")
            {
                // set condition jump to end of condition
                withers.top()->jump = prev->next;
                //std::cout << prev->next << std::endl;
                // set end of condition jump to start
                prev->jump = withers.top();
                withers.pop();
            }
            
            prev = prev->next;
            // for wither statement start condition
            if (prev->expression[0] == "wither") withers.push(prev);
        }
        parsedLine.clear();
        k++;
    }
    // set the last nodes next to a end of file token
    if (prev->expression[0] == "end")
    {
        // set condition jump to end of condition
        withers.top()->jump = NULL;
        // set end of condition jump to start
        prev->jump = withers.top();
        withers.pop();
    }
    prev->next = NULL;
}


void Interpreter::interpret()
{
    std::ifstream iFile;
    iFile.open(fileName);
    std::string line;
    // current index in code
    int i = 0;
    int j = 0;
    // for loops
    int loopStart = 0;
    bool looping = false;
    std::string looperName;
    std::string token;
    std::string temp;
    // graph traversal
    node* n = head;
    while (n != NULL)
    {
        //std::stringstream s(line);
        while (j < n->expression.size())
        {
            token = n->expression[j];
            //std::cout << token << std::endl;
            // comments
            if (token == "//")
            {
                n = n->next;
                break;
            }
            if (token == "TAB")
            {
                j++;
            }
            // the start of a loop
            if (token == "wither")
            {
                // next token is the loop length
                j++;
                token = n->expression[j];
                looperName = token; // store the name of token
                //loopStart = i;
                if (variables.find(token)->second > 0)
                {
                    n = n->next;
                    j = 0;
                    variables.find(token)->second--;
                    break;
                }
                n = n->jump;
                j = 0;
                break;
            }
            // loop end
            if (token == "end")
            //if (i > 0)
            //        if ((code[i-1][0]=="TAB")&&(code[i][0]!="TAB"))
            {
                n = n->jump;
                j = 0;
                break;
            }

            // initializing a variable
            if (token == "seed")
            {
                // get the name
                j++;
                token = n->expression[j];
                temp = token;
                // get the int
                j++;
                token = n->expression[j];
                // store the variable in the map
                variables.insert ( std::pair<std::string, int>(temp,stoi(token)));
                n = n->next;
                break;
            }
            // add to stack
            if (token == "plant")
            {
                // get the name
                j++;
                token = n->expression[j];
                garden.push(variables.find(token)->second);
                n = n->next;
                break;
            }
            // add var to top value and push new value
            if (token == "propagate")
            {
                // get the name
                j++;
                token = n->expression[j];
                pushChangedValue(token,1);
                n = n->next;
                break;
            }
            // pop from stack
            if (token == "dig")
            {
                int num = garden.top();
                garden.pop();
                
                j++;
                token = n->expression[j];
                // put the new value into the plant var 
                if (token == "into")
                {
                    j++;
                    token = n->expression[j];
                    variables.find(token)->second = num;
                }
                n = n->next;
                break;
            }

            // print
            if (token == "examine")
            {
                j++;
                token = n->expression[j];
                std::cout << variables.find(token)->second << std::endl;
                n = n->next;
                break;
            }
            // detect errors
            if (token != "TAB")
            {
                std::cout << "syntax error: " << token << std::endl;
                j++;
                token = n->expression[j];
                n = n->next;
                break;
            }
        }
        //if ((n->expression[0] != "Wither")&&(n->expression[0] != "End")) n = n->next;
        j=0;
    }
}
// helper functions

void Interpreter::printCode()
{
    node* n = head;
    while (n != NULL)
    {
        for (int i = 0; i < n->expression.size(); i++)
        {
            std::cout << n->expression[i] << " ";
        }
        std::cout << std::endl;
        n = n->next;
    }
}
// add or subtract plant to garden and push the value
void Interpreter::pushChangedValue(std::string s, int sign)
{
    int n = 0;
    if (s == "")
    {
        garden.push(garden.top());
        return;
    }
    // search for the variable added
    n = variables.find(s)->second;
    // add or subtract the values and push the new value to the garden
    garden.push(n + garden.top()*sign);
}