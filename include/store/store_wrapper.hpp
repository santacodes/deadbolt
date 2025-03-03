#ifndef TOTP_MANAGER_HPP
#define TOTP_MANAGER_HPP

#include <memory>
#include <stdexcept>
#include <string>

extern "C" {
#include "store.h"
}

/**
 * C++ wrapper for the TOTP Secret Manager
 */
class TOTPManager {
public:
  static void storeKey(const std::string &service_name,
                       const std::string &username,
                       const std::string &totp_key) {
    int result = totp_store_key(service_name.c_str(), username.c_str(),
                                totp_key.c_str());
    if (result != 0) {
      throw std::runtime_error("Failed to store TOTP key");
    }
  }

  static std::string retrieveKey(const std::string &service_name,
                                 const std::string &username) {
    char *key = totp_retrieve_key(service_name.c_str(), username.c_str());
    if (key == nullptr) {
      throw std::runtime_error("No TOTP key found");
    }

    std::string result(key);
    totp_free_key(key);
    return result;
  }

  static void deleteKey(const std::string &service_name,
                        const std::string &username) {
    int result = totp_delete_key(service_name.c_str(), username.c_str());
    if (result != 0) {
      throw std::runtime_error("Failed to delete TOTP key");
    }
  }

  static bool keyExists(const std::string &service_name,
                        const std::string &username) {
    char *key = totp_retrieve_key(service_name.c_str(), username.c_str());
    bool exists = (key != nullptr);
    if (exists) {
      totp_free_key(key);
    }
    return exists;
  }
};

#endif
