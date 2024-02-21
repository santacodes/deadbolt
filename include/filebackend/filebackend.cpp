#include <string>
#include <iostream>
#include <map>
#include "filebackend/filebackend.hpp"


filebackend::filebackend(std::map<std::string, std::string> *args) {

    this->functions = args;


}

void filebackend::create() {
    // Make a keyring to store the password hash and map them to the variable
    ;
}

void filebackend::append() {
    printf("Appending to the file..");
}

void filebackend::del() {
    printf("Deleting the file ");
}
