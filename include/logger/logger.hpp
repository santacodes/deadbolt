#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <mutex>
#include <string>

class Logger {
public:
  explicit Logger();
  ~Logger();
  void log(const std::string &message);

private:
  std::ofstream logFile;
  std::mutex logMutex;
};

#endif // LOGGER_HPP
