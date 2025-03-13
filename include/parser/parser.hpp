#include <string>
#include <unordered_map>
#ifndef PARSER_H
#define PARSER_H

class Parser {
public:
  static std::unordered_map<std::string, std::string> *
  options(int size, char *cmdinput[]);
};

#endif
