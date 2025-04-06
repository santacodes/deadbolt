// Compile this source only for linux
#ifdef __linux__

#include "store.h"
#include <libsecret/secret.h>
#include <stdio.h>

#define KEYRING_LABEL "deadbolt"
#define COLLECTION_NAME "/org/freedesktop/secrets/collection/deadbolt"
#define SCHEMA_NAME "org.example.deadbolt"

static const SecretSchema totp_schema = {
    .name = SCHEMA_NAME,
    .flags = SECRET_SCHEMA_NONE,
    .attributes = {
        {"service", SECRET_SCHEMA_ATTRIBUTE_STRING},
        {NULL, 0},
    }};

static SecretCollection *cached_collection = NULL;
static SecretService *cached_service = NULL;

static SecretCollection *get_or_create_collection() {
  if (cached_collection)
    return cached_collection;

  GError *error = NULL;
  if (!cached_service) {
    cached_service =
        secret_service_get_sync(SECRET_SERVICE_LOAD_COLLECTIONS, NULL, &error);
    if (!cached_service) {
      g_printerr("Failed to connect to Secret Service: %s\n", error->message);
      g_clear_error(&error);
      return NULL;
    }
  }

  GList *collections = secret_service_get_collections(cached_service);
  for (GList *l = collections; l != NULL; l = l->next) {
    SecretCollection *coll = SECRET_COLLECTION(l->data);
    const gchar *label = secret_collection_get_label(coll);
    if (g_strcmp0(label, KEYRING_LABEL) == 0) {
      cached_collection = g_object_ref(coll);
      break;
    }
  }
  g_list_free_full(collections, g_object_unref);

  if (!cached_collection) {
    g_print("Creating keyring '%s'...\n", KEYRING_LABEL);
    cached_collection = secret_collection_create_sync(
        cached_service, KEYRING_LABEL, NULL, SECRET_COLLECTION_CREATE_NONE,
        NULL, &error);
    if (!cached_collection) {
      g_printerr("Failed to create keyring: %s\n", error->message);
      g_clear_error(&error);
      return NULL;
    }
  }
  return cached_collection;
}

static int unlock_collection() {
  if (!cached_collection)
    return 0;
  if (!secret_collection_get_locked(cached_collection)) {
    g_print("Keyring already unlocked\n");
    return 1;
  }
  GError *error = NULL;
  GList *objects = g_list_append(NULL, cached_collection);
  GList *unlocked_objects = NULL;
  gboolean unlocked = secret_service_unlock_sync(cached_service, objects, NULL,
                                                 &unlocked_objects, &error);
  g_list_free(objects);

  if (error) {
    g_printerr("Failed to unlock keyring: %s\n", error->message);
    g_clear_error(&error);
  }
  g_list_free_full(unlocked_objects, g_object_unref);
  return unlocked ? 1 : 0;
}

int check_keyrings_exists() { return get_or_create_collection() != NULL; }

int totp_store_key(const char *service_name, const char *totp_key) {
  if (!check_keyrings_exists() || !unlock_collection())
    return 1;

  GError *error = NULL;
  gboolean result = secret_password_store_sync(
      &totp_schema, COLLECTION_NAME, service_name, totp_key, NULL, &error,
      "service", service_name, NULL);
  if (error) {
    g_printerr("Error storing TOTP key: %s\n", error->message);
    g_clear_error(&error);
  }
  return result ? 0 : 1;
}

char *totp_retrieve_key(const char *service_name) {
  if (!check_keyrings_exists() || !unlock_collection())
    return NULL;

  GError *error = NULL;
  char *totp_key = secret_password_lookup_sync(&totp_schema, NULL, &error,
                                               "service", service_name, NULL);
  if (error) {
    g_printerr("Error retrieving TOTP key: %s\n", error->message);
    g_clear_error(&error);
  }
  return totp_key;
}

void totp_free_key(char *totp_key) {
  if (totp_key)
    secret_password_free(totp_key);
}

int totp_delete_key(const char *service_name) {
  if (!check_keyrings_exists() || !unlock_collection())
    return 1;

  GError *error = NULL;
  gboolean result = secret_password_clear_sync(&totp_schema, NULL, &error,
                                               "service", service_name, NULL);
  if (error) {
    g_printerr("Error deleting TOTP key: %s\n", error->message);
    g_clear_error(&error);
  }
  return result ? 0 : 1;
}

#endif
