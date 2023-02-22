#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <unistd.h> //for (unix/linux)
#include "filebackend.hpp"

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

    int option;
    filebackend obj;
    // put ':' in the starting of the
    // string so that program can
    // distinguish between '?' and ':'
    while ((option = getopt(argc, argv, ":ced")) != -1)
    {
        switch (option)
        {
        case 'c':
            std::cout << "Creating a new note" << std::endl;

            obj.create();
            break;
        case 'e':

            std::cout << "Editing an existing note";
            break;
        case 'd':
            std::cout << "Deleting the note" << std::endl
                      << option;
            break;
        case 'f':
            std::cout << "filename: %s " << std::endl
                      << optarg;
            break;
        case ':':
            std::cout << "option needs a value" << std::endl;
            break;
        case '?':
            // printf(“unknown option: %c\n”, optopt);
            break;
        default:
            exit(0);
            break;
        }
    }

    // optind is for the extra arguments
    // which are not parsed
    for (; optind < argc; optind++)
    {
        std::cout << "extra arguments: " << std::endl
                  << argv[optind];
    }

    

    return 0;
};
