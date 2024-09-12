#include <keyutils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEYRING "deadbolt"

void createkey(char keyname[], char keyhash[]) {
  // Make a keyring to store the password hash and map them to the variable
  // https://man7.org/linux/man-pages/man2/add_key.2.html
  key_serial_t keyring;
  key_serial_t key;
  key_serial_t requested_key;
  const char *keyring_name = KEYRING;
  const char *key_name = keyname;
  const char *key_data = keyhash;
  size_t key_data_len = sizeof(*key_data);

  // Create a new keyring
  keyring = add_key("keyring", keyring_name, NULL, 0, KEY_SPEC_USER_KEYRING);
  if (keyring == -1) {
    perror("add_key (keyring)");
    exit(EXIT_FAILURE);
  }

  printf("Created keyring with serial: %d\n", keyring);

  // Add a key to the keyring
  key = add_key("user", key_name, key_data, key_data_len, keyring);
  if (key == -1) {
    perror("add_key (key)");
    exit(EXIT_FAILURE);
  }

  printf("Added key with serial: %d\n", key);
}
