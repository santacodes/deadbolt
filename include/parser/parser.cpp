#include "parser/parser.hpp"
#include <iostream>
#include <store/store_wrapper.hpp>
#include <string>
#include <unordered_map>
#include <vector>

// Global vector of stored arguments
std::vector<std::string> args;
std::unordered_map<std::string, std::string> processed_argument;
// Global vector of nodes for arguments storage

std::unordered_map<std::string, std::string> *
Parser::options(int size, char *cmdinput[]) {

  /* Parser Options method is used to get the arguments of command line input
     Returns a vector of arguments passed in command line */

  std::string arg;
  std::vector<std::string> argument_vector;

  // Making a hashmap of all the argument and argument values from the
  // string

  for (int i = 0; i < size; i++) {

    if (cmdinput[i][0] == '-' ||
        (cmdinput[i][0] == '-' && cmdinput[i][1] == '-')) {

      arg = cmdinput[i];
      arg.erase(0, 1);
      if (cmdinput[i][1] == '-')
        arg.erase(0, 1);
      args.push_back(arg);

      for (int j = i + 1; j < size; j++) {
        if (*cmdinput[j] == '-') {
          argument_vector.clear();
          break;
        } else {

          argument_vector.push_back(cmdinput[j]);

          // change the string vector into one string and append to the
          // map value
          processed_argument[arg] += cmdinput[j];
        }
      }
    }
  }

  args.clear();
  std::cout << "This is the hashmap" << std::endl;
  for (auto x : processed_argument)
    std::cout << x.first << " " << x.second
              << std::endl; // returns the hashmap of the key and argument
  Parser::perform_operations(&processed_argument);
  return &processed_argument;
}

int Parser::perform_operations(
    std::unordered_map<std::string, std::string> *ops) {
  // n/new for new service
  // g/get for get totp
  // l/list to get all the totp
  if (((*ops)["n"] != "" || (*ops)["new"] != "") &&
      ((*ops)["s"] != "" || (*ops)["secret"] != "")) {
    std::string service = (*ops)["n"] != "" ? (*ops)["n"] : (*ops)["new"];
    std::string secret = (*ops)["s"] != "" ? (*ops)["s"] : (*ops)["secret"];
    SecretsManager::storeKey(service, secret);
  } else if (((*ops)["d"] != "" || (*ops)["delete"] != "")) {
    std::string service = (*ops)["d"] != "" ? (*ops)["d"] : (*ops)["delete"];
    SecretsManager::deleteKey(service);
  }
  return 1;
}
