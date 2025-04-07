/*
 * This is a SIMD implementation of HMAC-SHA1 based TOTP generation algorithm.
 * For reference refer RFC - 6238 https://datatracker.ietf.org/doc/html/rfc6238.
 * This file computes multiple TOTPs using vectorized batches of secrets with
 * the same instruction set. While this might not impact performance a lot on a
 * personal computer, it might create a significant difference in a distributed
 * system.
 * Anyway, a SIMD instruction would reduce the number of cycles in the CPU
 * hence reducing the energy consumption and number of instruction sets overall.
 *
 */

#include <algorithm>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <logger/logger.hpp>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <sstream>
#include <string>
#include <totp/totp.hpp>
#include <vector>
#include <xsimd/xsimd.hpp>

namespace xs = xsimd;

Logger logger;

// Vectorized base32 decoding
std::vector<uint8_t> totp::decodeBase32(std::string input) {
  // Base32 character set (RFC 4648)
  const std::string base32Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

  // Normalize input: remove spaces and handle padding
  input.erase(std::remove_if(input.begin(), input.end(), ::isspace),
              input.end());

  std::vector<uint8_t> result;
  result.reserve((input.length() * 5) / 8);
  uint64_t buffer = 0;
  int bitsLeft = 0;

  for (char c : input) {
    if (c == '=')
      continue; // Skip padding

    size_t val = base32Chars.find(std::toupper(c));
    if (val == std::string::npos)
      continue; // Skip invalid characters

    buffer = (buffer << 5) | val;
    bitsLeft += 5;

    if (bitsLeft >= 8) {
      bitsLeft -= 8;
      result.push_back((buffer >> bitsLeft) & 0xFF);
    }
  }

  return result;
}

// Vectorized dynamic truncation
uint32_t
totp::dynamicTruncateVectorized(const std::vector<uint8_t> &hmacResult) {

  size_t offset = hmacResult.back() & 0x0F;

  if (offset + 4 <= hmacResult.size()) {
    using batch_type = xs::batch<uint8_t, xs::default_arch>;

    alignas(xs::default_arch::alignment()) std::array<uint8_t, batch_type::size>
        aligned_bytes;
    std::fill(aligned_bytes.begin(), aligned_bytes.end(), 0);

    for (size_t i = 0; i < 4; ++i) {
      aligned_bytes[i] = hmacResult[offset + i];
    }

    auto batch = xs::load_aligned(aligned_bytes.data());

    uint8_t first_byte_masked = aligned_bytes[0] & 0x7F;

    uint32_t binary = (static_cast<uint32_t>(first_byte_masked) << 24) |
                      (static_cast<uint32_t>(aligned_bytes[1]) << 16) |
                      (static_cast<uint32_t>(aligned_bytes[2]) << 8) |
                      static_cast<uint32_t>(aligned_bytes[3]);

    return binary;
  } else {
    // Fallback for edge cases
    uint32_t binary = ((hmacResult[offset] & 0x7F) << 24) |
                      ((hmacResult[offset + 1] & 0xFF) << 16) |
                      ((hmacResult[offset + 2] & 0xFF) << 8) |
                      (hmacResult[offset + 3] & 0xFF);

    return binary;
  }
}

std::vector<uint8_t> totp::hmacSha1(const std::vector<uint8_t> &key,
                                    const std::vector<uint8_t> &message) {
  std::vector<uint8_t> digest(EVP_MAX_MD_SIZE);
  size_t digestLength = 0;

#if OPENSSL_VERSION_NUMBER >= 0x30000000L
  // OpenSSL 3.0 and later
  EVP_MAC *mac = EVP_MAC_fetch(NULL, "HMAC", NULL);
  EVP_MAC_CTX *ctx = EVP_MAC_CTX_new(mac);

  OSSL_PARAM params[2];
  params[0] =
      OSSL_PARAM_construct_utf8_string("digest", const_cast<char *>("sha1"), 0);
  params[1] = OSSL_PARAM_construct_end();

  EVP_MAC_init(ctx, key.data(), key.size(), params);
  EVP_MAC_update(ctx, message.data(), message.size());
  EVP_MAC_final(ctx, digest.data(), &digestLength, digest.size());

  EVP_MAC_CTX_free(ctx);
  EVP_MAC_free(mac);
#else
  // OpenSSL 1.1.1 and earlier
  HMAC_CTX *ctx = HMAC_CTX_new();
  HMAC_Init_ex(ctx, key.data(), key.size(), EVP_sha1(), nullptr);
  HMAC_Update(ctx, message.data(), message.size());
  HMAC_Final(ctx, digest.data(), (unsigned int *)&digestLength);
  HMAC_CTX_free(ctx);
#endif

  digest.resize(digestLength);
  return digest;
}

// Vectorized generation of counter bytes
std::vector<uint8_t> totp::generateCounterBytes(uint64_t counter) {
  // Create aligned storage for SIMD operations
  using batch_type = xs::batch<uint8_t, xs::default_arch>;
  alignas(xs::default_arch::alignment()) std::array<uint8_t, 8>
      aligned_counter = {0};

  for (int i = 7; i >= 0; i--) {
    aligned_counter[i] = counter & 0xFF;
    counter >>= 8;
  }

  return std::vector<uint8_t>(aligned_counter.begin(), aligned_counter.end());
}

// Vectorized TOTP generation
std::string totp::generateTOTP(const std::string &base32Secret,
                               uint64_t timeStep, uint64_t t0, int digits) {

  time_t currentTime = time(nullptr);
  uint64_t counter = (currentTime - t0) / timeStep;

  std::vector<uint8_t> key = decodeBase32(base32Secret);

  std::vector<uint8_t> message = generateCounterBytes(counter);

  std::vector<uint8_t> hmacResult = hmacSha1(key, message);

  uint32_t truncatedHash = dynamicTruncateVectorized(hmacResult);

  uint32_t hotp = truncatedHash % static_cast<uint32_t>(std::pow(10, digits));

  std::stringstream ss;
  ss << std::setw(digits) << std::setfill('0') << hotp;

  return ss.str();
}

// Batch processing of multiple TOTPs using SIMD
std::vector<std::string>
totp::generateBatchTOTP(const std::vector<std::string> &secrets,
                        uint64_t timeStep, uint64_t t0, int digits) {
  std::vector<std::string> results;
  results.reserve(secrets.size());

  time_t currentTime = time(nullptr);
  uint64_t counter = (currentTime - t0) / timeStep;

  std::vector<uint8_t> counterBytes = generateCounterBytes(counter);

  for (const auto &secret : secrets) {
    results.push_back(generateTOTP(secret, timeStep, t0, digits));
  }
  return results;
}

int getTimeLeft() {
  time_t currentTime = time(nullptr);
  uint64_t timeStep = 30;
  uint64_t counter = currentTime / timeStep;
  int time_left = timeStep - (currentTime % timeStep);
  return time_left;
}

// Debug function only to test, Do not use in release!
void totp::printDebugInfo() {
  time_t currentTime = time(nullptr);
  uint64_t timeStep = 30;
  uint64_t counter = currentTime / timeStep;

  std::cout << "Current time: " << currentTime << std::endl;
  std::cout << "Counter value: " << counter << std::endl;
  std::cout << "Time in period: " << currentTime % timeStep << "s" << std::endl;
  std::cout << "Time until next code: " << timeStep - (currentTime % timeStep)
            << "s" << std::endl;

  std::cout << "\nSIMD features available:" << std::endl;

  std::cout << "Default architecture: " << xs::default_arch::name()
            << std::endl;
  std::cout << "Vector size in bytes: "
            << xs::batch<uint8_t, xs::default_arch>::size << std::endl;
}

int totp::fetch_totps(std::string &secret) {

  std::cout << "TOTP: " << generateTOTP(secret) << std::endl;
  std::cout << "Time until next code : " << getTimeLeft() << std::endl;
  return 0;
}

int totp::fetch_totps(std::vector<std::string> &secrets) {

  // Check if a secret was provided as a command-line argument
  if (secrets.size() == 0) {
    std::cout << "No secrets provided!";
    return 1;
  }
  if (secrets.size() == 1) {
    std::string otp = generateTOTP(secrets[0]);
    std::cout << "Current TOTP: " << otp << std::endl;
    std::cout << "Time until next code : " << getTimeLeft() << std::endl;
  }
  // Process a vector batch of secrets to generate TOTP
  if (secrets.size() > 2) {

    std::cout << "\nBatch processing multiple secrets:" << std::endl;
    auto batchResults = generateBatchTOTP(secrets);

    for (size_t i = 0; i < batchResults.size(); i++) {
      std::cout << "TOTP for secret " << i + 1 << ": " << batchResults[i]
                << std::endl;
    }
    std::cout << "Time until next code : " << getTimeLeft() << std::endl;
  }
  // printDebugInfo();

  return 0;
}
