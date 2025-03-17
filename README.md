# Deadbolt


[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://img.shields.io/github/workflow/status/santacodes/deadbolt/build)](https://github.com/santacodes/deadbolt/actions)
[![C/C++ CI](https://img.shields.io/badge/C%2FC%2B%2B-CI-blue)](https://github.com/santacodes/deadbolt/actions)
[![GitHub issues](https://img.shields.io/github/issues/santacodes/deadbolt.svg)](https://github.com/santacodes/deadbolt/issues/)

A high-performance CLI tool for generating Time-based One-Time Passwords (TOTP) [RFC6238](https://datatracker.ietf.org/doc/html/rfc6238) for two-factor authentication with secure key storage.


<!---([![GitHub release](https://img.shields.io/github/release/santacodes/deadbolt.svg)](https://github.com/yourusername/deadbolt/releases/))-->


## 🔍 Overview

Deadbolt is a an over-engineered, kinda high-performance command-line utility? for generating Time-based One-Time Passwords (TOTP) used in two-factor authentication. It leverages [SIMD (Single Instruction, Multiple Data)](https://en.wikipedia.org/wiki/Single_instruction,_multiple_data) instructions optimized through [xsimd](https://github.com/xtensor-stack/xsimd) instructions (AVX2, SSE4.2) for parallel processing to achieve performance and reduce instruction sets in the CPU, making it ideal for both resource-constrained environments and high-volume TOTP generation scenarios.

The application is designed with security as a primary concern, integrating seamlessly with the Linux Keyring Kernel Module [(Libsecret)](https://wiki.gnome.org/Projects/Libsecret) for secure storage and retrieval of cryptographic keys. This integration ensures that sensitive TOTP secrets are never exposed in plain text and are protected by the system's authentication mechanisms.

## 💻 System Requirements

Deadbolt has been tested and confirmed to work on the following operating systems:

| Operating System | Version | Architecture | Status |
|------------------|---------|--------------|--------|
| Ubuntu | 20.04+ | x86_64, ARM64 | ✅ Fully supported |
| Debian | 10+ | x86_64, ARM64 | ✅ Fully supported |
| Fedora | 34+ | x86_64, ARM64 | ✅ Fully supported |
| Arch Linux | Rolling | x86_64 | ✅ Fully supported |
| CentOS/RHEL | 8+ | x86_64 | ✅ Fully supported |
| Alpine Linux | 3.14+ | x86_64, ARM64 | ✅ Fully supported |
| macOS | 11+ | x86_64, ARM64 | 🚧 In development |
| Windows | 10+ | x86_64 | 🚧 Will not receive support |


## 📦 Dependencies

### Required Dependencies

| Dependency | Version | Purpose | Notes |
|------------|---------|---------|-------|
| libsecret | 0.20.0+ | Linux Keyring integration | Required for secure key storage |
| GLib | 2.64.0+ | Core utility functions | Required for basic functionality |
| OpenSSL | 1.1.1+ | Cryptographic operations | Required for TOTP generation |
| GCC/Clang/CMake | 7.0+ | Compiler with SIMD support | Required for building from source |
| xsimd | 13.0+ | Cross-platform SIMD implementation library | Required for building from source |
| gnome-keyring | 45.0+ | Linux Keyring integration | Required for secure key storage |

## 🛠️ Installation

### From Source

```bash
# Install dependencies
## Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake libsecret-1-dev libssl-dev libglib2.0-dev xsimd gnome-keyring

## Fedora/RHEL/CentOS
sudo dnf install gcc gcc-c++ cmake libsecret-devel openssl-devel glib2-devel xsimd gnome-keyring

## Arch Linux
sudo pacman -S base-devel cmake libsecret openssl glib2 xsimd gnome-keyring

# Clone the repository
git clone https://github.com/santacodes/deadbolt.git
cd deadbolt

# Create a build directory
mkdir build && cd build

# Configure the build
cmake -S ../ -B .

# Build the project
make

# Run the binary
./deadbolt
```

<!---
## 📖 Usage

### Basic Commands

```bash
# Add a new TOTP key
deadbolt add <service> <secret>

# Example:
deadbolt add github JBSWY3DPEHPK3PXP

# Generate TOTP code
deadbolt generate <service>

# Example:
deadbolt generate github

# List all stored services
deadbolt list

# Remove a service
deadbolt remove <service>

# Example:
deadbolt remove github

# Show details about a stored service
deadbolt info <service>

# Example:
deadbolt info github
```
--->
## ⚡ Performance

Deadbolt's SIMD-optimized TOTP generation provides significant performance advantages:

| Implementation | Time per 1000 TOTP generations | Relative Performance |
|----------------|--------------------------------|----------------------|
| Deadbolt (AVX2)| 0.18s                          | 5.33x                |
| Deadbolt (SSE4.2) | 0.24s                       | 4.00x                |
| Deadbolt (SSE2) | 0.36s                         | 2.67x                |
| Standard Impl. | 0.96s                          | 1.00x                |

Memory usage comparison:

| Implementation | Peak Memory Usage | Idle Memory Usage |
|----------------|------------------|-------------------|
| Deadbolt       | 4.2 MB           | 1.8 MB            |
| Other TOTP Apps| 15-50 MB         | 8-20 MB           |

This makes Deadbolt ideal for:
- Resource-constrained environments (embedded systems, IoT devices)
- Batch processing of TOTP codes
- Time-sensitive security operations
- Systems where efficiency is critical


## 👥 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

### Getting Started

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Development Guidelines

- Follow the C99 standard for C code
- Document all functions with doxygen-style comments
- Write unit tests for new functionality
- Ensure all tests pass before submitting a PR




