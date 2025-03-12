#include "store.h"
#include <libsecret/secret.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define APP_NAME "/org/freedesktop/secrets/collection/deadbolt"
#define SCHEMA_NAME "org.example.deadbolt"

// Global schema definition for deadbolt
static const SecretSchema totp_schema = {
    .name = SCHEMA_NAME,
    .flags = SECRET_SCHEMA_NONE,
    .attributes = {
        {"service", SECRET_SCHEMA_ATTRIBUTE_STRING},
        {NULL, 0},
    }};

// Callback function for collection creation
static void on_collection_created(GObject *source, GAsyncResult *res,
                                  gpointer user_data) {
  GError *error = NULL;
  SecretCollection *collection = secret_collection_create_finish(res, &error);

  if (error) {
    g_printerr("Failed to create keyring: %s\n", error->message);
    g_clear_error(&error);
  } else {
    g_print("Custom keyring 'deadbolt' created successfully!\n");
  }

  if (collection) {
    g_object_unref(collection);
  }
}
int check_keyrings_exists() {
  GError *error = NULL;

  SecretService *service =
      secret_service_get_sync(SECRET_SERVICE_OPEN_SESSION, NULL, &error);
  if (!service) {
    g_printerr("Failed to connect to Secret Service: %s\n", error->message);
    g_clear_error(&error);
    return 1;
  }

  GList *collections = secret_service_get_collections(service);
  gboolean keyring_exists = FALSE;

  for (GList *l = collections; l != NULL; l = l->next) {
    SecretCollection *collection = SECRET_COLLECTION(l->data);
    const gchar *label = secret_collection_get_label(collection);

    if (g_strcmp0(label, APP_NAME) == 0) {
      keyring_exists = TRUE;
      break;
    }
  }

  // Create keyring only if it does not exist
  if (!keyring_exists) {
    secret_collection_create(service,
                             APP_NAME, // Keyring name
                             NULL,     // Keyring name alias
                             SECRET_COLLECTION_CREATE_NONE, NULL,
                             on_collection_created, &error);

    if (error) {
      g_printerr("Failed to create collection: %s\n", error->message);
      g_clear_error(&error);
      return 0;
    } else {
      g_print("First time setup: Keyring created successfully!\n");
      return 1;
    }
  } else {
    // g_print("Keyring 'my_custom_keyring' already exists. Skipping
    // creation.\n");
    return 1;
  }

  // Clean up
  g_object_unref(service);
  return 0;
}

int totp_store_key(const char *service_name, const char *totp_key) {
  GError *error = NULL;
  gboolean result;

  result = secret_password_store_sync(&totp_schema, // The schema
                                      APP_NAME,     // Default collection
                                      service_name, // Label for the secret
                                      totp_key,     // The secret (TOTP key)
                                      NULL,         // No cancellable object
                                      &error,       // Error pointer
                                      "service",
                                      service_name, // Attribute: service name
                                      NULL          // End of attributes
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

char *totp_retrieve_key(const char *service_name) {
  GError *error = NULL;
  char *totp_key;

  totp_key = secret_password_lookup_sync(
      &totp_schema,            // The schema
      NULL,                    // No cancellable object
      &error,                  // Error pointer
      "service", service_name, // Attribute: service name
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

int totp_delete_key(const char *service_name) {
  GError *error = NULL;
  gboolean result;

  result = secret_password_clear_sync(&totp_schema, // The schema
                                      NULL,         // No cancellable object
                                      &error,       // Error pointer
                                      "service",
                                      service_name, // Attribute: service name
                                      NULL          // End of attributes
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
