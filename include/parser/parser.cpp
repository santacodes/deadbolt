#include <iostream>
#include <string>
#include "parser/parser.hpp"

// Parser Options method is used to get the arguments of command line input 
int parser::options(int size, char *cmdinput[]) {
	
	std::string arg;

	for(int i = 0; i < size	; i++) {

		if(cmdinput[i][0] == '-') {
			
			arg = cmdinput[i];
			arg.erase(0,1);
			std::cout << arg << std::endl;

		}
	}

	return 0;
}

