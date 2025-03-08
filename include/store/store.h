#ifndef STORE_H
#define STORE_H

#ifdef __cplusplus
extern "C" {
#endif

int check_keyrings_exists();

int totp_store_key(const char *service_name, const char *totp_key);

char *totp_retrieve_key(const char *service_name);

void totp_free_key(char *totp_key);

int totp_delete_key(const char *service_name);

#ifdef __cplusplus
}
#endif

#endif // STORE_H
