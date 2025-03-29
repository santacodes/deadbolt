#include "logger.hpp"
#include <chrono>
#include <ctime>

#define FILENAME "log.txt"

// Initialise with a log file
Logger::Logger() {
  logFile.open(FILENAME, std::ios::app);
  if (!logFile) {
    throw std::runtime_error("Unable to open log file");
  }
}

Logger::~Logger() {
  if (logFile.is_open()) {
    logFile.close();
  }
}

void Logger::log(const std::string &message) {
  std::lock_guard<std::mutex> lock(logMutex);
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  logFile << "[" << std::ctime(&now_time) << "] " << message << std::endl;
}
