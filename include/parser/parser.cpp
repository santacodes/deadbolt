#include <iostream>
#include <string>
#include <vector>
#include "parser/parser.hpp"


// Global vector of stored arguments 
std::vector<std::string> args; 

// Global vector of nodes for arguments storage

std::vector<parser::node> nodes;

std::vector<std::string>* parser::options(int size, char *cmdinput[]) {
	
/* Parser Options method is used to get the arguments of command line input 
   Returns a vector of arguments passed in command line */

	std::string arg;
	parser::node node;

	for(int i = 0; i < size; i++) {

		if(cmdinput[i][0] == '-' || (cmdinput[i][0] == '-' && cmdinput[i][1] == '-')) {
			
			arg = cmdinput[i];
			arg.erase(0,1);
			if(cmdinput[i][1] == '-') arg.erase(0,1);
			args.push_back(arg);
			node.argument = arg;
			nodes.push_back(node);
    
		}

	}

	return &args;
}

