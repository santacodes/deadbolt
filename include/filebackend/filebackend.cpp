#include <string>
#include <iostream>
#include "filebackend/filebackend.hpp"
#include "sqlite/sqlite3.h"


filebackend::filebackend() {
    sqlite3 *DB;
    std::string sql = "CREATE TABLE PASSWORDS("
                        "ID INT PRIMARY KEY     NOT NULL, "
                        "USERNAME           TEXT    NOT NULL, "
                        "PASSWORD          TEXT     NOT NULL;";
    int exit = 0;
    exit = sqlite3_open("pwd.db", &DB);
    char *messaggeError;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

    if (exit != SQLITE_OK)
    {
        std::cerr << "Error Create Table" << std::endl;
        sqlite3_free(messaggeError);
    }
    else
        std::cout << "Table created Successfully" << std::endl;
    sqlite3_close(DB);
    std::cout << "Closed";

}

void filebackend::create() {   
    std::string entry = "INSERT INTO TABLE PASSWORDS("
                        "ID INT PRIMARY KEY     NOT NULL, "
                        "USERNAME           TEXT    NOT NULL, "
                        "PASSWORD          TEXT     NOT NULL;";
    printf("created the file");
}

void filebackend::append() {
    printf("Appending to the file..");
}

void filebackend::del() {
    printf("Deleting the file ");
}
