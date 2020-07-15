#include "Interpreter.h"

Interpreter::Interpreter(std::string file)
{
    fileName = file;
}

// parse the file
void Interpreter::parse()
{
    std::ifstream iFile;
    iFile.open(fileName);
    std::string line;
    std::string token;
    std::vector<std::string> parsedLine;
    while(getline(iFile,line))
    {
        std::stringstream s(line);
        while (getline(s,token,' '))
        {
            parsedLine.push_back(token);
        }
        code.push_back(parsedLine);
        parsedLine.clear();
    }
}


void Interpreter::interpret()
{
    std::ifstream iFile;
    iFile.open(fileName);
    std::string line;
    // current index in code
    int i, j = 0;
    // for loops
    int loopStart = 0;
    bool looping = false;
    std::string looperName;

    std::string token;
    std::string temp;
    while(i < code.size())
    {
        std::stringstream s(line);
        while (j < code[i].size())
        {
            token = code[i][j];

            // comments
            if (token == "//") break;
            // initializing a variable
            if (token == "Seed")
            {
                // get the name
                j++;
                token = code[i][j];
                temp = token;
                // get the int
                j++;
                token = code[i][j];
                // store the variable in the map
                variables.insert ( std::pair<std::string, int>(temp,stoi(token)));
                break;
            }
            // add to stack
            if (token == "Plant")
            {
                // get the name
                j++;
                token = code[i][j];
                garden.push(variables.find(token)->second);
                break;
            }
            // add var to top value and push new value
            if (token == "Propigate")
            {
                // get the name
                j++;
                token = code[i][j];
                pushChangedValue(token,1);
                break;
            }
            // subtract var to top value and push new value
            if (token == "Trim")
            {
                // get the name
                j++;
                token = code[i][j];
                pushChangedValue(token,-1);
                break;
            }
            // pop from stack
            if (token == "Dig")
            {
                int n = garden.top();
                garden.pop();
                
                j++;
                token = code[i][j];
                // put the new value into the plant var 
                if (token == "into")
                {
                    j++;
                    token = code[i][j];
                    variables.find(token)->second = n;
                }
                break;
            }
            // the start of a loop
            if (token == "Water")
            {
                // next token is the loop length
                j++;
                token = code[i][j];
                looperName = token; // store the name of token
                loopStart = i;
                break;
            }
            // loop end
            if (token == "Finish")
            {
                variables.find(looperName)->second--;
                if (variables.find(looperName)->second > 0)
                {
                    i = loopStart;
                }
                break;
            }


            // print
            if (token == "Examine")
            {
                j++;
                token = code[i][j];
                std::cout << variables.find(token)->second << std::endl;
                break;
            }
        }
        i++;
        j=0;
    }
}
// helper functions

void Interpreter::gardenToString()
{
    std::string s;
}
// add or subtract plant to garden and push the value
void Interpreter::pushChangedValue(std::string s, int sign)
{
    int n = 0;
    // search for the variable added
    n = variables.find(s)->second;
    // add or subtract the values and push the new value to the garden
    garden.push(n + garden.top()*sign);
}
