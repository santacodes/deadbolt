#ifndef TOTP_HPP
#define TOTP_HPP

#include <algorithm>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <sstream>
#include <string>
#include <vector>
#include <xsimd/xsimd.hpp>

namespace xs = xsimd;
class totp {
private:
  std::vector<uint8_t> decodeBase32(std::string input);
  uint32_t dynamicTruncateVectorized(const std::vector<uint8_t> &hmacResult);
  std::vector<uint8_t> hmacSha1(const std::vector<uint8_t> &key,
                                const std::vector<uint8_t> &message);
  std::vector<uint8_t> generateCounterBytes(uint64_t counter);
  std::string generateTOTP(const std::string &base32Secret,
                           uint64_t timeStep = 30, uint64_t t0 = 0,
                           int digits = 6);
  std::vector<std::string>
  generateBatchTOTP(const std::vector<std::string> &secrets,
                    uint64_t timeStep = 30, uint64_t t0 = 0, int digits = 6);
  void printDebugInfo();

public:
  int fetch_totps(std::vector<std::string> &secrets);
  int fetch_totps(std::string &secret);
};
#endif // TOTP_HPP
