#include <iostream>
#include <fstream>
#include <vector>
#include <string>



/*class cli{
	public:

};*/


int main(){
	std::string choice;
	std::cout << "Enter the operation you want to perform : " << std::endl;
	std::cout << "C - Create a new file" << std::endl;
	std::cout << "E - Edit a file" << std::endl;
	std::cout << "D - Delete a file" << std::endl;
	std::cin >> choice;
	std::cout << "your choice is " << choice << std::endl;
	return 0;
};
