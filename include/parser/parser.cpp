#include <iostream>
#include <string>
#include <vector>
#include "parser/parser.hpp"

std::vector<std::string> args; 

std::vector<std::string>* parser::options(int size, char *cmdinput[]) {
	
// Parser Options method is used to get the arguments of command line input 
// Returns a vector of arguments passed in command line

	
	std::vector<std::string>::iterator ptr; 
	std::string arg;
	

	for(int i = 0; i < size; i++) {

		if(cmdinput[i][0] == '-') {
			
			arg = cmdinput[i];
			arg.erase(0,1);
			std::cout << arg << std::endl;
			args.push_back(arg);
    
      
    // Displaying vector elements using begin() and end() 
    std::cout << "The vector elements are : "; 
    for(ptr = args.begin(); ptr < args.end(); ptr++) 
        std::cout << *ptr << " "; 

		}
	}

	return &args;
}

