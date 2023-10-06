#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <map>
#include "filebackend/filebackend.hpp"
#include "sqlite/sqlite3.h"
/*

--- DOCUMENTATION ---

class options{
    bool --help;
    std::string --greeting = "Hello";
    unsigned int --exclamations = 1;
std::cout << "List of arguments - " << std::endl;
                std::cout << "-c, -C, -create => To Create a new Encrypted note" << std::endl;
                std::cout << "-e, -E, -edit => To edit an existing note" << std::endl;
                std::cout << "-d, -D, -delete => To delete an exsiting note" << std::endl;


};*/

// Command line arguments for cli
int main(int argc, char *argv[])
{

    std::cout << "works";

//   filebackend obj;
//    obj.append();
    // Parser 
    std::cout << "Argument count" << argc << std::endl;
    std::cout << "The Arguments" << std::endl;
    filebackend obj;
    obj.create();
    for(int i = 0; i < argc; i++) {

        std::cout << argv[i] << std::endl;


    } 
    return 0;
};
