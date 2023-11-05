#include <string>
#include <vector>
#ifndef PARSER
#define PARSER 


class parser {
public: 
	
	struct node{

		std::string argument;
		std::string note;

	};

	std::vector<std::string>* options(int size,char *cmdinput[]);
	
};

#endif