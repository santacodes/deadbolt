#include "filebackend/filebackend.hpp"
#include "parser/parser.hpp"
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <map>
#include <string>
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

  // Only run the TUI when there are no arguments mentioned
  if (argc < 2) {
    auto screen = ftxui::Screen::Create(ftxui::Dimension::Fixed(32),
                                        ftxui::Dimension::Fixed(10));

    auto &pixel = screen.PixelAt(9, 9);
    pixel.character = U'D';
    pixel.bold = true;
    pixel.foreground_color = ftxui::Color::Blue;

    std::cout << screen.ToString();
    return EXIT_SUCCESS;
    std::cout << "Invalid Operation, use `Deadbolt --help` to get started";

  }

  else {

    std::map<std::string, std::string> args;
    std::vector<std::string>::iterator ptr;
    parser par;

    // Processing the arguments and returning a string array of the args
    args = *par.options(argc, argv);
    filebackend fbackend(&args);

    // Calling the backend to store and make an entry
  }

  return 0;
};
