#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

/*

--- DOCUMENTATION ---

class options{
	bool --help;
  	std::string --greeting = "Hello";
  	unsigned int --exclamations = 1;

std::cout << "Enter the operation you want to perform : " << std::endl;
	std::cout << "C - Create a new file" << std::endl;
	std::cout << "E - Edit a file" << std::endl;
	std::cout << "D - Delete a file" << std::endl;
	std::cin >> choice;
	std::cout << "your choice is " << choice << std::endl;


};*/

//Command line arguments for cli
int main(int argc, char *argv[]){
	
	//std::string choice;
	
	if(argc == 0) std::cout << "NO ARGUMENTS WERE PROVIDED";

	else{
		switch(argv[0]){
		case "-help":
			if(argv[0] == "-help"){
				std::cout << "List of arguments - " << std::endl;
				std::cout << "-c, -C, -create => To Create a new Encrypted note" << std::endl;
				std::cout << "-e, -E, -edit => To edit an existing note" << std::endl;
				std::cout << "-d, -D, -delete => To delete an exsiting note" << std::endl;
			}
		};

		for(int i = 0; i < argc; i++) {

			std::cout << argv[i] << std::endl;
		};


	}

	//td::cout << "this is argc" << argc << "this is argv" << argv;
	
	
	return 0;
};
