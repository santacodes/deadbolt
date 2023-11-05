#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <map>
#include "filebackend/filebackend.hpp"
#include "sqlite/sqlite3.h"
#include "parser/parser.hpp"
/*

--- DOCUMENTATION ---

class options{
    bool --help;
    std::string --greeting = "Hello";
    unsigned int --exclamations = 1;
std::cout << "List of arguments - " << std::endl;
                std::cout << "-c, -C, --create => To Create a new Encrypted note" << std::endl;
                std::cout << "-e, -E, --edit => To edit an existing note" << std::endl;
                std::cout << "-d, -D, --delete => To delete an exsiting note" << std::endl;


};*/

// Command line arguments for cli
int main(int argc, char *argv[])
{

    std::vector<std::string> args;
    std::vector<std::string>::iterator ptr; 
    parser par;
    args = *par.options(argc, argv);
    
    for(ptr = args.begin(); ptr < args.end(); ptr++) 
        std::cout << *ptr << " ";


    return 0;
};
