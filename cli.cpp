#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <unistd.h>

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

//Command line arguments for cli
int main(int argc, char *argv[]){
	
	int opt;
      
    // put ':' in the starting of the
    // string so that program can 
    //distinguish between '?' and ':' 
    while((opt = getopt(argc, argv,":if:lrx")) != -1) 
    { 
        switch(opt) 
        { 
            case '-': std::cout << "single hyphen"; 
            case 'l': 
            case 'r': 
                std::cout << "option: %c" << std::endl << opt; 
                break; 
            case 'f': 
                std::cout << "filename: %s " << std::endl << optarg; 
                break; 
            case ':': 
                std::cout << "option needs a value" << std::endl; 
                break; 
            case '?': 
                //printf(“unknown option: %c\n”, optopt);
                break; 
        } 
    } 
      
    // optind is for the extra arguments
    // which are not parsed
    for(; optind < argc; optind++){     
        std::cout << "extra arguments: "<< std::endl << argv[optind]; 
    }
      
  
	
	
	return 0;
};
