cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(deadbolt LANGUAGES CXX C)

find_package(OpenSSL REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSECRET REQUIRED libsecret-1)
pkg_check_modules(GLIB REQUIRED glib-2.0)

# Search simd paths
set(XSIMD_SEARCH_PATHS
    ${XSIMD_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/external/xsimd
    ${CMAKE_CURRENT_SOURCE_DIR}/external/xsimd/include
    ${CMAKE_CURRENT_SOURCE_DIR}/.local/include/xsimd
    /usr/local/include/xsimd
    /usr/include/xsimd
)

# Find xsimd header
find_path(XSIMD_INCLUDE_DIR 
    NAMES xsimd/xsimd.hpp
    PATHS ${XSIMD_SEARCH_PATHS}
    PATH_SUFFIXES include
)

# Check if xsimd was found
if(NOT XSIMD_INCLUDE_DIR)
    message(FATAL_ERROR "xsimd headers not found. Please specify XSIMD_PATH or ensure xsimd is installed.")
endif()

# Include directories
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# TOTP library
add_library(totp STATIC 
    ${INCLUDE_DIR}/totp/totp.cpp
)
target_include_directories(totp PUBLIC 
    ${INCLUDE_DIR}
    ${XSIMD_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
)
target_link_libraries(totp PUBLIC 
    OpenSSL::Crypto
)

# TOTP key storage with libsecret
add_library(store STATIC 
    ${INCLUDE_DIR}/store/store.c
)
target_include_directories(store PUBLIC 
    ${INCLUDE_DIR}
    ${LIBSECRET_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
)
target_link_libraries(store PUBLIC 
    ${LIBSECRET_LIBRARIES}
    ${GLIB_LIBRARIES}
)

# Parser library
add_library(parser STATIC 
    ${INCLUDE_DIR}/parser/parser.cpp
)
target_include_directories(parser PUBLIC 
    ${INCLUDE_DIR}
    ${XSIMD_INCLUDE_DIR}
)
target_link_libraries(parser PUBLIC 
    store 
    totp
)

link_directories(${LIBSECRET_LIBRARY_DIRS} ${GLIB_LIBRARY_DIRS})

# Main executable
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/cli.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${INCLUDE_DIR}
    ${LIBSECRET_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${XSIMD_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
) 
target_link_libraries(${PROJECT_NAME} PRIVATE 
    parser
    OpenSSL::Crypto
)
