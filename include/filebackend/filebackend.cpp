#include "filebackend/filebackend.hpp"
#include <iostream>
#include <keyapi/keyapi.h>
#include <map>
#include <stdio.h>

filebackend::filebackend(std::map<std::string, std::string> *args) {

  this->functions = *args;

  // Doing Checks here to validate the argument data
  if (functions["n"] != "" || functions["new"] != "") {
    std::cout << "work in progress";
#ifdef __cplusplus
    extern "C" {
#endif

    // your normal definitions here

#ifdef __cplusplus
    }
#endif

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

void filebackend::append() { printf("Appending to the file.."); }

void filebackend::del() { printf("Deleting the file "); }
