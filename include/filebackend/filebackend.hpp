#include <keyutils.h>
#include <map>
#include <string>
#include <vector>
#ifndef FILEBACKEND
#define FILEBACKEND

class filebackend {
public:
  filebackend(std::map<std::string, std::string> *);

private:
  void create();
  void append();
  void del();
  std::map<std::string, std::string> functions;
};

#endif
