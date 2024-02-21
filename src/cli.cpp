#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "filebackend/filebackend.hpp"
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
    std::cout << "Welcome to Deadbolt!" << std::endl;

    if (argc < 2) std::cout << "Invalid Operation, use `Deadbolt --help` to get started";
    else {

        std::map<std::string, std::string> args;
        std::vector<std::string>::iterator ptr;
        parser par;

        
        // Processing the arguments and returning a string array of the args
        args = *par.options(argc, argv);
        filebackend fbackend(&args); 

        // Calling the backend to store and make an entry
        

    }

    return 0;
};
