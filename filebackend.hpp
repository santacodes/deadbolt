#pragma once
// To create and edit in the sqlite db
#include <string>
#include <sqlite3.h>

class filebackend
{
public:
    filebackend()
    {
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
    };

    void create()
    {
        printf("created the file");
    };
    void append()
    {
        printf("Appending to the file..");
    };
    void del()
    {
        printf("Deleting the file ");
    };
};