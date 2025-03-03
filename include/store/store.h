#ifndef TOTP_SECRET_MANAGER_H
#define TOTP_SECRET_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

int totp_store_key(const char *service_name, const char *username,
                   const char *totp_key);

char *totp_retrieve_key(const char *service_name, const char *username);

void totp_free_key(char *totp_key);

int totp_delete_key(const char *service_name, const char *username);

#ifdef __cplusplus
}
#endif

#endif
