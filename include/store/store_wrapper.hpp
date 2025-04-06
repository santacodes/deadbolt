#ifndef TOTP_MANAGER_HPP
#define TOTP_MANAGER_HPP

#include <stdexcept>
#include <string>

extern "C" {
#include "store.h"
}

/**
 * C++ wrapper for the TOTP Secret Manager
 */
class SecretsManager {
public:
  /**
   * Check if the deadbolt secret service keyring exists
   * @throws std::runtime_error if storing the key fails
   * @return int, 1 if exits or 0/error if it doesn't
   */
  static int Check_Keyring_Exists() {
    int result = check_keyrings_exists();
    if (result == 1) {
      return result;
    } else {
      throw std::runtime_error("Failed to connect deadbolt collections");
      return result;
    }
    return result;
  }
  /**
   * Store a TOTP key for a given service
   * @param service_name Name of the service
   * @param totp_key TOTP secret to store
   * @throws std::runtime_error if storing the key fails
   */
  static void storeKey(const std::string &service_name,
                       const std::string &totp_key) {
    int result = totp_store_key(service_name.c_str(), totp_key.c_str());
    if (result != 0) {
      throw std::runtime_error("Failed to store TOTP key for service: " +
                               service_name);
    }
  }

  /**
   * Retrieve a TOTP key for a given service
   * @param service_name Name of the service
   * @return TOTP secret
   * @throws std::runtime_error if no key is found
   */
  static std::string retrieveKey(const std::string &service_name) {
    char *key = totp_retrieve_key(service_name.c_str());
    if (key == nullptr) {
      throw std::runtime_error("No TOTP key found for service: " +
                               service_name);
    }
    std::string result(key);
    totp_free_key(key);
    return result;
  }

  /**
   * Delete a TOTP key for a given service
   * @param service_name Name of the service
   * @throws std::runtime_error if deletion fails
   */
  static void deleteKey(const std::string &service_name) {
    int result = totp_delete_key(service_name.c_str());
    if (result != 0) {
      throw std::runtime_error("Failed to delete TOTP key for service: " +
                               service_name);
    }
  }

  /**
   * Lists the available services
   */
  static void listServices() {
    int result = list_services();
    if (result != 0) {
      throw std::runtime_error("Failed to list the services");
    }
  }

  /**
   * Check if a TOTP key exists for a given service
   * @param service_name Name of the service
   * @return true if key exists, false otherwise
   */
  static bool keyExists(const std::string &service_name) {
    char *key = totp_retrieve_key(service_name.c_str());
    bool exists = (key != nullptr);
    if (exists) {
      totp_free_key(key);
    }
    return exists;
  }
};

#endif // TOTP_MANAGER_HPP
