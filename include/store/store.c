#include <libsecret-1/libsecret/secret.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Schema for storing TOTP keys
static const SecretSchema TOTP_KEY_SCHEMA = {
    "com.example.TOTP.Key",
    SECRET_SCHEMA_NONE,
    {
        {"account", SECRET_SCHEMA_ATTRIBUTE_STRING},
        {"NULL", 0},
    }};

// Function to store a TOTP key in Libsecret
void store_totp_key(const char *account, const char *key) {
  GError *error = NULL;

  // Store the key securely in Libsecret
  secret_password_store_sync(&TOTP_KEY_SCHEMA, SECRET_COLLECTION_DEFAULT,
                             "TOTP Key", key,
                             NULL, // Cancellable
                             &error, "account", account,
                             NULL); // End of attributes

  if (error != NULL) {
    fprintf(stderr, "Error storing key: %s\n", error->message);
    g_error_free(error);
  } else {
    printf("Key for account '%s' stored successfully.\n", account);
  }
}

// Function to fetch a TOTP key from Libsecret
char *fetch_totp_key(const char *account) {
  GError *error = NULL;

  // Retrieve the key securely from Libsecret
  char *key = secret_password_lookup_sync(&TOTP_KEY_SCHEMA,
                                          NULL, // Cancellable
                                          &error, "account", account,
                                          NULL); // End of attributes

  if (error != NULL) {
    fprintf(stderr, "Error fetching key: %s\n", error->message);
    g_error_free(error);
    return NULL;
  }

  if (key == NULL) {
    printf("No key found for account '%s'.\n", account);
    return NULL;
  }

  printf("Key for account '%s' retrieved successfully.\n", account);
  return key;
}

int getKey(char Site[]) {
  char *key = fetch_totp_key(Site);
  if (key) {
    printf("Retrieved Key 1: %s\n", key);
  }

  return 0;
}
// have to add error handling
int Store(char Site[], char key[]) {
  // Store TOTP keys
  store_totp_key(Site, key);
  return 0;
}
