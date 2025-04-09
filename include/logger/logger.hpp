#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <mutex>
#include <string>

class Logger {
public:
  static Logger &get(); // Singleton accessor
  void log(const std::string &message);

private:
  std::ofstream logFile;
  std::mutex logMutex;
  Logger();
  ~Logger();
  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;
};

#endif // LOGGER_HPP
