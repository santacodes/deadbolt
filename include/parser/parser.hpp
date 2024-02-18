#include <string>
#include <vector>
#include <map>
#ifndef PARSER
#define PARSER


class parser {
public:

	std::map<std::string, std::string>* options(int size, char *cmdinput[]);

};

#endif