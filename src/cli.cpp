#include <iostream>
#include <logger/logger.hpp>
#include <parser/parser.hpp>
#include <store/store_wrapper.hpp>
#include <string>
#include <totp/totp.hpp>
#include <unordered_map>
/*

--- DOCUMENTATION ---

Operations that can be performed

  New entry (-n || --new, -s || --secret) : To create a entry to store a totp
secret
  Get (-g || --get) : Get TOTP for a service
  Delete (-d || --delete) : To delete the key

*/

// Command Line Interface
int main(int argc, char *argv[]) {
  if (argc < 1) {
    // start the tui here
    std::cout << "Welcome to Deadbolt TUI!" << std::endl;
  }

  std::cout << "Welcome to Deadbolt CLI!" << std::endl;
  std::unordered_map<std::string, std::string> *args;

  args = Parser::options(argc, argv);

  Logger::get().log("Checking if keyring exists");
  SecretsManager::Check_Keyring_Exists();

  return 0;
};
