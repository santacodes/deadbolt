// Compile this source only for MacOS
#ifdef __APPLE__

#include "store.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define DB_FILENAME "totp_keys.db"

static char *get_db_path() {
  const char *home_dir = getenv("HOME");
  if (!home_dir)
    return NULL;

  const char *base_dir = "/.local/share/totp_store/";
  size_t path_len =
      strlen(home_dir) + strlen(base_dir) + strlen(DB_FILENAME) + 1;
  char *db_path = (char *)malloc(path_len);
  if (!db_path)
    return NULL;

  snprintf(db_path, path_len, "%s%s%s", home_dir, base_dir, DB_FILENAME);

  char dir_path[strlen(home_dir) + strlen(base_dir) + 1];
  snprintf(dir_path, sizeof(dir_path), "%s%s", home_dir, base_dir);
  mkdir(dir_path, 0755);

  return db_path;
}

static sqlite3 *open_db() {
  char *db_path = get_db_path();
  if (!db_path)
    return NULL;

  sqlite3 *db = NULL;
  if (sqlite3_open(db_path, &db) != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    free(db_path);
    return NULL;
  }
  free(db_path);
  return db;
}

static int execute_sql(sqlite3 *db, const char *sql) {
  char *err_msg = NULL;
  int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQLite error: %s\n", err_msg);
    sqlite3_free(err_msg);
    return 0;
  }
  return 1;
}

int check_keyrings_exists() {
  sqlite3 *db = open_db();
  if (!db)
    return 0;

  const char *sql = "CREATE TABLE IF NOT EXISTS totp_keys (service TEXT "
                    "PRIMARY KEY, key TEXT NOT NULL);";
  int result = execute_sql(db, sql);
  sqlite3_close(db);
  return result;
}

int totp_store_key(const char *service_name, const char *totp_key) {
  sqlite3 *db = open_db();
  if (!db)
    return 0;

  const char *sql = "INSERT INTO totp_keys (service, key) VALUES (?1, ?2) ON "
                    "CONFLICT(service) DO UPDATE SET key=excluded.key;";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  sqlite3_bind_text(stmt, 1, service_name, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, totp_key, -1, SQLITE_STATIC);

  int result = (sqlite3_step(stmt) == SQLITE_DONE);
  sqlite3_finalize(stmt);
  sqlite3_close(db);
  return result;
}

char *totp_retrieve_key(const char *service_name) {
  sqlite3 *db = open_db();
  if (!db)
    return NULL;

  const char *sql = "SELECT key FROM totp_keys WHERE service = ?1;";
  sqlite3_stmt *stmt;
  char *key = NULL;

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
    sqlite3_bind_text(stmt, 1, service_name, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
      const char *retrieved_key = (const char *)sqlite3_column_text(stmt, 0);
      key = strdup(retrieved_key);
    }
  }

  sqlite3_finalize(stmt);
  sqlite3_close(db);
  return key;
}

void totp_free_key(char *totp_key) { free(totp_key); }

int totp_delete_key(const char *service_name) {
  sqlite3 *db = open_db();
  if (!db)
    return 0;

  const char *sql = "DELETE FROM totp_keys WHERE service = ?1;";
  sqlite3_stmt *stmt;
  int result = 0;

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
    sqlite3_bind_text(stmt, 1, service_name, -1, SQLITE_STATIC);
    result = (sqlite3_step(stmt) == SQLITE_DONE);
  }

  sqlite3_finalize(stmt);
  sqlite3_close(db);
  return result;
}

#endif
