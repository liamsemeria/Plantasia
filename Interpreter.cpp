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
    iFile.open(fileName);
    std::string line;
    std::string token;
    std::vector<std::string> parsedLine;
    // stack to count loop layers
    std::stack<node*> withers;
    node* gardenernode;
    int k = 0;
    node* prev = NULL;
    while(getline(iFile,line))
    {
        // if line is empty continue
        if (line == "") continue;
        // for code
        token = "";
        for (auto &ch : line)
        {
            if (ch == ' ' && token != "")
            {
                parsedLine.push_back(token);
                //dont need to read in comments
                if (token == "//") continue;
                token = "";
            } else if (ch != ' ' && ch != '\t') // ignore start spaces & tabs
            {
                token+=ch;
            }
        }
        parsedLine.push_back(token);
        token = "";
        // connect head
        if (!k)
        {
            head->expression = parsedLine;
            prev = head;
        }
        else 
        {
            //std::cout << prev->expression[0] << std::endl;
            //make nodes and wire them to the previous line
            prev->next = new node();
            prev->next->expression = parsedLine;
            //std::cout << parsedLine[0] << std::endl;
            // function definition
            if (prev->expression[0] == "gardener")
            {
                // add function start pointer
                functions.insert(std::pair<std::string, node*>(prev->expression[1],prev));
                gardenernode = prev;
            }
            // end function definition
            if (prev->expression[0] == "finish")
            {
                // set jump to nextline after function call
                prev->jump = gardenernode->next;
                // skip garden def during interpret time
                gardenernode->jump = prev->next;
            }
            // wither end statement
            if (prev->expression[0] == "end")
            {
                // set condition jump to end of condition if no dead statement
                if (withers.size() > 0 && withers.top()->jump == NULL)
                {
                    //std::cout << "SAVE ME" << std::endl;
                    withers.top()->jump = prev->next;
                    prev->jump = withers.top();
                    withers.pop();
                }
                //std::cout << prev->next << std::endl;
                // set end of condition jump to start
            }
            // wither dead statement
            if (prev->expression[0] == "dead")
            {
                // set condition jump to end of condition
                withers.top()->jump = prev->next;
                //std::cout << "SHIT URSELF" << std::endl;
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
    // temp number
    int number = 0;
    // for loops
    int loopStart = 0;
    bool looping = false;
    std::string looperName;
    std::string token;
    std::string temp;
    int witherjumps = 0;
    // graph traversal
    node* n = head;
    // call stack
    std::stack<node*> callstack;
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
                // SUS CODE SUS CODE SUS CODE
                // skip death statement if iterator didnt start at 0
                if (variables.find(token)->second == 0 && witherjumps != 0) n = n->jump->next;
                else n = n->jump;
                j = 0;
                break;
            }
            // loop end
            if (token == "end")
            {
                n = n->jump;
                j = 0;
                break;
            }
            // loop dead
            if (token == "dead")
            {
                witherjumps++;
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
                if (n->expression.size() == j) number = 0;
                else number = stoi(n->expression[j]);
                // store the variable in the map
                variables.insert ( std::pair<std::string, int>(temp,number));
                n = n->next;
                break;
            }
            // add to stack
            if (token == "plant")
            {
                // get the name
                j++;
                token = n->expression[j];
                if (variables.contains(token)) garden.push(variables.find(token)->second);
                else garden.push(stoi(token));
                n = n->next;
                break;
            }
            // add var to top value and push new value
            if (token == "propagate")
            {
                // get the name
                j++;
                token = n->expression[j];
                j++;
                if (token.length() == 0) pushChangedValue("",'@');
                else pushChangedValue(n->expression[j],token[0]);
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
		        bool format = 0;
		        // print as char if @ is before val
		        if (token == "@") 
		        {
		            format = 1;
		            j++;
		            token = n->expression[j];
		        }
                if (format)
                {
                    if (token == "garden") printf("%c",garden.top());//std::cout << garden.top() << std::endl;
                    else printf("%c",variables.find(token)->second);
                } else {
                    if (token == "garden") printf("%d",garden.top());//std::cout << garden.top() << std::endl;
                    else printf("%d",variables.find(token)->second);//std::cout << variables.find(token)->second << std::endl;
                }
                n = n->next;
                break;
            }
            if (token == "absorb")
            {
                if (inputnum < inputs.size())
                {
                    garden.push(inputs[inputnum]);
                    inputnum++;
                }
                n = n->next;
                break;
            }
            // gardener skip??
            if (token == "gardener")
            {
                n = n->jump;
                break;
            }
            // finish
            if (token == "finish" || token == "teleport")
            {
                n = callstack.top()->next;
                callstack.pop();
                break;
            }
            // END PROGRAM
            if (token == "EXIT")
            {
                return;
            }
            // detect errors
            if (token != "TAB")
            {
                // check if token is a function
                if (functions.contains(token))
                {
                    callstack.push(n);
                    n = functions.find(token)->second->next;
                    break;
                }
                else
                {
                    std::cout << "syntax error: " << token << std::endl;
                    j++;
                    token = n->expression[j];
                    n = n->next;
                    break;
                }
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
void Interpreter::pushChangedValue(std::string s, char opp)
{
    int n = 0;
    if (s == "")
    {
        garden.push(garden.top());
        return;
    }
    // search for the variable added
    if (variables.contains(s))
        n = variables.find(s)->second;
    else if (opp != '!')//n = stoi(s);
        try {n = stoi(s);} catch (std::invalid_argument e) {
            std::cout << "propagate input not a variable or integer: " << s << std::endl;
        }
    // add or subtract the values and push the new value to the garden
    switch (opp)
    {
        case '+': garden.push(garden.top() + n); break;
        case '-': garden.push(garden.top() - n); break;
        case '*': garden.push(garden.top() * n); break;
        case '/': garden.push(garden.top() / n); break;
        case '%': garden.push(garden.top() % n); break;
        // comparisons SUS CODE TEMPORARY ADD 2 CHAR STUFF
        case '=': garden.push(garden.top() == n); break;
        case '>': garden.push(garden.top() > n); break;
        case '<': garden.push(garden.top() < n); break;
        case '!': garden.push(! garden.top()); break;
        default: garden.push(garden.top() + n); break;
    }
}
