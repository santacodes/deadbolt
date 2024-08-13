#include <map>
#include <string>
#ifndef PARSER_H
#define PARSER_H

class parser {
public:
  std::map<std::string, std::string> *options(int size, char *cmdinput[]);
};

#endif
