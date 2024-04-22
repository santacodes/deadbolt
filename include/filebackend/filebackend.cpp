#include "filebackend/filebackend.hpp"
#include <iostream>
#include <keyutils.h>
#include <map>
#include <string>

filebackend::filebackend(std::map<std::string, std::string> *args) {

  this->functions = *args;

  // Doing Checks here to validate the argument data
  if (functions["n"] != "" || functions["new"] != "") {
    std::cout << "work in progress";
  } else
    std::cout << "No file name mentioned";

  if (functions["e"] != "" || functions["edit"] != "")
    std::cout << "Edit file not null";
  else
    ;

  if (functions["l"] != "" || functions["list"] != "")
    std::cout << "List file not null";
  else
    ;

  if (functions["d"] != "" || functions["delete"] != "")
    std::cout << "Delete file not null";
  else
    ;
}

void filebackend::create() {
  // Make a keyring to store the password hash and map them to the variable
  // https://man7.org/linux/man-pages/man2/add_key.2.html

  int keyvalue =
      add_key("user", "this is a sample key", "facebook key", size_t 20, 10239);
  if (keyvalue == 0)
    std::cout << "error";
  else
    std::cout << "created a keyring successfully";
}

void filebackend::append() { printf("Appending to the file.."); }

void filebackend::del() { printf("Deleting the file "); }
