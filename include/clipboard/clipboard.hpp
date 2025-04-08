#ifndef CLIPBOARD_HPP
#define CLIPBOARD_HPP
#include <iostream>

class Clipboard {
public:
  static int set_clipboard(std::string);
  static int get_clipboard();
};

#endif
