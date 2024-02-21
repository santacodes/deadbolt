#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "parser/parser.hpp"


// Global vector of stored arguments
std::vector<std::string> args;
std::map<std::string, std::string> processed_argument;
// Global vector of nodes for arguments storage

std::map<std::string, std::string>* parser::options(int size, char *cmdinput[]) {

	/* Parser Options method is used to get the arguments of command line input
	   Returns a vector of arguments passed in command line */

	std::string arg;
	std::vector<std::string> argument_vector;


// Sliding window to detect the operation and arguments of the operation and
// storing it in a node like hashmap

	for (int i = 0; i < size; i++) {

		if (cmdinput[i][0] == '-' || (cmdinput[i][0] == '-' && cmdinput[i][1] == '-')) {

			arg = cmdinput[i];
			arg.erase(0, 1);
			if (cmdinput[i][1] == '-') arg.erase(0, 1);
			args.push_back(arg);

			for (int j = i + 1; j < size; j++) {
				if (*cmdinput[j] == '-') {
					argument_vector.clear();
					break;
				}
				else {

					argument_vector.push_back(cmdinput[j]);

					// change the string vector into one string and append to the hashmap value
					processed_argument[arg] += cmdinput[j];

				}
			}

		}

	}

	args.clear();
	std::cout << "This is the hashmap" << std::endl;
	for (auto x : processed_argument)
		std::cout << x.first << " " << x.second << std::endl;
	
	return &processed_argument;
}

