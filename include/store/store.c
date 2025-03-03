#include "store.h"
#include <libsecret/secret.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define APP_NAME "deadbolt"
#define SCHEMA_NAME "org.deadbolt.totp"
#define DEFAULT_COLLECTION "default"

// Global schema definition for deadbolt
static const SecretSchema totp_schema = {
    .name = SCHEMA_NAME,
    .flags = SECRET_SCHEMA_NONE,
    .attributes = {
        {"service", SECRET_SCHEMA_ATTRIBUTE_STRING},
        {"username", SECRET_SCHEMA_ATTRIBUTE_STRING},
        {NULL, 0},
    }};

int totp_store_key(const char *service_name, const char *username,
                   const char *totp_key) {
  GError *error = NULL;
  gboolean result;

  result = secret_password_store_sync(
      &totp_schema,            // The schema
      DEFAULT_COLLECTION,      // Default collection
      service_name,            // Label for the secret
      totp_key,                // The secret (TOTP key)
      NULL,                    // No cancellable object
      &error,                  // Error pointer
      "service", service_name, // Attribute: service name
      "username", username,    // Attribute: username
      NULL                     // End of attributes
  );

  if (error != NULL) {
    fprintf(stderr, "Error storing TOTP key: %s\n", error->message);
    g_error_free(error);
    return 1;
  }

  if (!result) {
    fprintf(stderr, "Failed to store TOTP key for unknown reason\n");
    return 1;
  }

  return 0;
}

char *totp_retrieve_key(const char *service_name, const char *username) {
  GError *error = NULL;
  char *totp_key;

  totp_key = secret_password_lookup_sync(
      &totp_schema,            // The schema
      NULL,                    // No cancellable object
      &error,                  // Error pointer
      "service", service_name, // Attribute: service name
      "username", username,    // Attribute: username
      NULL                     // End of attributes
  );

  if (error != NULL) {
    fprintf(stderr, "Error retrieving TOTP key: %s\n", error->message);
    g_error_free(error);
    return NULL;
  }

  return totp_key; // Caller must free this with totp_free_key()
}

void totp_free_key(char *totp_key) {
  if (totp_key != NULL) {
    secret_password_free(totp_key);
  }
}

int totp_delete_key(const char *service_name, const char *username) {
  GError *error = NULL;
  gboolean result;

  result = secret_password_clear_sync(
      &totp_schema,            // The schema
      NULL,                    // No cancellable object
      &error,                  // Error pointer
      "service", service_name, // Attribute: service name
      "username", username,    // Attribute: username
      NULL                     // End of attributes
  );

  if (error != NULL) {
    fprintf(stderr, "Error deleting TOTP key: %s\n", error->message);
    g_error_free(error);
    return 1;
  }

  if (!result) {
    return 1;
  }

  return 0;
}
