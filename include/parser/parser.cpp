#include "parser/parser.hpp"
#include <iostream>
#include <store/store_wrapper.hpp>
#include <string>
#include <totp/totp.hpp>
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
  Parser::perform_operations(&processed_argument);
  return &processed_argument;
}

int Parser::perform_operations(
    std::unordered_map<std::string, std::string> *ops) {
  totp totp_inst = totp();

  std::string name = !(*ops)["n"].empty() ? (*ops)["n"] : (*ops)["new"];
  std::string secret = !(*ops)["s"].empty() ? (*ops)["s"] : (*ops)["secret"];
  std::string del = !(*ops)["d"].empty() ? (*ops)["d"] : (*ops)["delete"];
  std::string get = !(*ops)["g"].empty() ? (*ops)["g"] : (*ops)["get"];

  if (!name.empty() && !secret.empty()) {
    SecretsManager::storeKey(name, secret);
  } else if (!del.empty()) {
    SecretsManager::deleteKey(del);
  } else if (!get.empty()) {
    std::string retrievedSecret = SecretsManager::retrieveKey(get);
    std::cout << "This is the TOTP for: " << get << std::endl;
    totp_inst.fetch_totps(retrievedSecret);
  } else {
    std::cout << "Invalid CLI argument(s)!" << std::endl;
  }

  return 0;
}
