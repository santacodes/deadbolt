#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <unordered_map>

class Parser {
public:
  static std::unordered_map<std::string, std::string> *
  options(int size, char *cmdinput[]);

private:
  static int
  perform_operations(std::unordered_map<std::string, std::string> *ops);
};

#endif
