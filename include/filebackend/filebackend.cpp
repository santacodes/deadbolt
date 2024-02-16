#include <string>
#include <iostream>
#include "filebackend/filebackend.hpp"
#include "sqlite/sqlite3.h"


filebackend::filebackend(std::vector<std::string> *args) {

    std::cout << "works";
    this->functions = args;
    std::vector<std::string>::iterator ptr; 
    for(ptr = args->begin(); ptr < args->end(); ptr++) {
        std::cout << *ptr << std::endl;
    }
    
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
