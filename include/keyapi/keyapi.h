#ifndef KEYAPI_H
#define KEYAPI_H
#include <keyutils.h>
#include <libsecret-1/libsecret/secret.h>
#include <stdio.h>
#include <string.h>

void createkey(char keyname[], char keyhash[]);
#define EXAMPLE_SCHEMA example_get_schema()
#endif
