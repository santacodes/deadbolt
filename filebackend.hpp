#pragma once
//To create and edit in the sqlite db
#include <string>
#include <sqlite3>


class filebackend{
public: 
	sqlite3* DB;
    std::string sql = "CREATE TABLE PASSWORDS("
                      "ID INT PRIMARY KEY     NOT NULL, "
                      "USERNAME           TEXT    NOT NULL, "
                      "PASSWORD          TEXT     NOT NULL;";
void create(){
	printf("created the file");

	}
void append(){
	printf("Appending to the file..");

}	
void del(){
	printf("Deleting the file ")

}

};