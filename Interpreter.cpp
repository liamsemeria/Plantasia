#include "Interpreter.h"


Interpreter::Interpreter(std::string file)
{
    fileName = file;
}

// parse the file
void Interpreter::interpret()
{
    std::ifstream iFile;
    iFile.open(fileName);
    std::string line;
    std::string token;
    std::string temp;
    while(getline(iFile,line))
    {
        std::stringstream s(line);
        while (getline(s,token,' '))
        {
            // initializing a variable
            if (token == "Seed")
            {
                getline(s,token,' '); // get the name
                temp = token;
                getline(s,token,' '); // get the int
                // store the variable in the map
                variables.insert ( std::pair<std::string, int>(temp,stoi(token)));
                break;
            }
            // add to stack
            if (token == "Plant")
            {
                getline(s,token,' '); // get the name
                garden.push(variables.find(token)->second);
                break;
            }
            // add var to top value and push new value
            if (token == "Propigate")
            {
                getline(s,token,' '); // get the name
                pushChangedValue(token,1);
                break;
            }
            // subtract var to top value and push new value
            if (token == "Trim")
            {
                getline(s,token,' '); // get the name
                pushChangedValue(token,-1);
                break;
            }
            // pop from stack
            if (token == "Dig")
            {
                int n = garden.top();
                garden.pop();
                getline(s,token,' ');
                // put the new value into the plant var 
                if (token == "into")
                {
                    getline(s,token,' ');
                    variables.find(token)->second = n;
                }
                break;
            }
            // the start of a loop
            if (token == "Water")
            {
                // next token is the loop length
                getline(s,token,' ');
                std::string name = token;

                // while the plant is not 0, execute the loop
                while (variables.find(name)->second > 0)
                {
                    variables.find(name)->second--;
                    // find and execute the loop body
                    // TODO
                }
            }

            // print
            if (token == "Examine")
            {
                getline(s,token,' ');
                std::cout << variables.find(token)->second << std::endl;
            }

        }
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