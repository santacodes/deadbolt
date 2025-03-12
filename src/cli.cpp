#include <iostream>
#include <map>
#include <parser/parser.hpp>
#include <store/store_wrapper.hpp>
#include <string>
#include <totp/totp.hpp>
#include <vector>
/*

--- DOCUMENTATION ---

Operations that can be performed

  New entry (-n || --new) : To create a new entry to store a key
  List (-l || --list) : To list all the entries present
  Edit (-e || --edit) : To edit the already present keys
  Delete (-e || --delete) : To delete the key

};*/

// Command line arguments for cli
int main(int argc, char *argv[]) {
  std::cout << "Welcome to Deadbolt!" << std::endl;

  std::vector<std::string> secrets = {
      "2FASTEST", "YESMAN",
      "NOMAN"}; // Initialise a vector and pass it to get the otps
  totp obj = totp();
  obj.fetch_totps(secrets);
  SecretsManager::Check_Keyring_Exists();
  SecretsManager::storeKey("github", "SECRETKEY123");
  std::string key = SecretsManager::retrieveKey("github");
  std::cout << "This is the key" << key << std::endl;
  std::map<std::string, std::string> args;
  std::vector<std::string>::iterator ptr;
  parser par;

  // Processing the arguments and returning a string array of the args
  args = *par.options(argc, argv);
  // filebackend fbackend(&args);

  // Calling the backend to store and make an entry

  return 0;
};
