#include <immintrin.h>    // For AVX2 intrinsics
#include <omp.h>          // For OpenMP
#include <openssl/hmac.h> // For HMAC-SHA1/SHA256
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTP_DIGITS 6
#define TIME_STEP 30

typedef enum { SHA1, SHA256 } HashAlgorithm;

void counter_to_big_endian(uint64_t counter, uint8_t *buffer) {
  for (int i = 7; i >= 0; i--) {
    buffer[i] = counter & 0xFF;
    counter >>= 8;
  }
}

void hmac(const uint8_t *key, size_t key_len, const uint8_t *message,
          size_t message_len, uint8_t *output, HashAlgorithm algorithm) {
  unsigned int len =
      (algorithm == SHA1)
          ? 20
          : 32; // Output length: 20 bytes for SHA-1, 32 for SHA-256
  const EVP_MD *md = (algorithm == SHA1) ? EVP_sha1() : EVP_sha256();
  HMAC(md, key, key_len, message, message_len, output, &len);
}

// Generate TOTP codes using OpenMP and HMAC
void generate_totp(const uint8_t *keys[], size_t key_lengths[],
                   HashAlgorithm algorithms[], uint64_t current_time,
                   uint32_t results[], size_t num_keys) {
  const uint32_t mod_divisor = 1000000; // For 6-digit TOTP
  uint64_t time_step = current_time / TIME_STEP;

  // Allocate memory for counters and outputs
  uint8_t(*counters)[8] = malloc(num_keys * sizeof(uint8_t[8]));
  uint8_t(*outputs)[32] =
      malloc(num_keys * sizeof(uint8_t[32])); // 32 bytes for SHA-256 max output

#pragma omp parallel for
  for (size_t i = 0; i < num_keys; i++) {
    counter_to_big_endian(time_step, counters[i]);
  }

#pragma omp parallel for
  for (size_t i = 0; i < num_keys; i++) {
    hmac(keys[i], key_lengths[i], counters[i], sizeof(counters[i]), outputs[i],
         algorithms[i]);
  }

#pragma omp parallel for
  for (size_t i = 0; i < num_keys; i++) {
    uint8_t *hash = outputs[i];
    int offset =
        hash[31] &
        0x0F; // Dynamic offset based on last byte (SHA-256 has 32 bytes)
    uint32_t binary =
        ((hash[offset] & 0x7F) << 24) | ((hash[offset + 1] & 0xFF) << 16) |
        ((hash[offset + 2] & 0xFF) << 8) | (hash[offset + 3] & 0xFF);
    results[i] = binary % mod_divisor;
  }

  free(counters);
  free(outputs);
}

int get_totp(uint8_t *keys[], size_t *key_lengths[],
             HashAlgorithm *hash_algorithms[]) {
  // Give the input as array of keys for SIMD and OpenMP parallel processing
  // Example keys for 4 accounts
  uint32_t *results[4];
  const uint8_t *keys[4] = {
      (const uint8_t *)"2FASTEST",
      (const uint8_t *)"abcdefabcdefabcdefabcdefabcdefabcdef",
      (const uint8_t
           *)"0123456789012345678901234567890123456789012345678901234567890123",
      (const uint8_t *)"deadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbee"
                       "fdeadbeef"};
  // Get the current Unix timestamp
  uint64_t current_time = (uint64_t)time(NULL);
  // Generate TOTP codes
  generate_totp(*keys, *key_lengths, *hash_algorithms, current_time, *results,
                4);

  // Print results
  printf("Current Unix Time: %llu\n", current_time);
  for (int i = 0; i < 4; i++) {
    printf("TOTP %d: %06u\n", i + 1, results[i]);
  }

  return 0;
}
