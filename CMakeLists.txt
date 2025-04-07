cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(deadbolt LANGUAGES CXX C)

find_package(OpenSSL REQUIRED)
find_package(PkgConfig REQUIRED)

# Check platform for key storage
if(UNIX AND NOT APPLE)
    pkg_check_modules(LIBSECRET REQUIRED libsecret-1)
    pkg_check_modules(GLIB REQUIRED glib-2.0)
elseif(APPLE)
    find_package(SQLite3 REQUIRED)
endif()

# Search SIMD paths
set(XSIMD_SEARCH_PATHS
    ${XSIMD_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/xsimd
    ${CMAKE_CURRENT_SOURCE_DIR}/xsimd/include
    ${CMAKE_CURRENT_SOURCE_DIR}/.local/include/xsimd
    /usr/local/include/xsimd
    /usr/include/xsimd
)

# Find xsimd headers
find_path(XSIMD_INCLUDE_DIR 
    NAMES xsimd/xsimd.hpp
    PATHS ${XSIMD_SEARCH_PATHS}
    PATH_SUFFIXES include
)

if(NOT XSIMD_INCLUDE_DIR)
    message(FATAL_ERROR "xsimd headers not found. Please specify XSIMD_PATH or ensure xsimd is installed.")
endif()

# Include directories
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIR})

# Optimization flags
add_compile_options(-O3)

# TOTP library
add_library(totp STATIC 
    ${INCLUDE_DIR}/totp/totp.cpp
)
target_include_directories(totp PUBLIC 
    ${INCLUDE_DIR}
    ${XSIMD_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
)
target_link_libraries(totp PUBLIC OpenSSL::Crypto)

# Key Storage (Platform-Specific)
# Libsecret/keyrings for Linux 
if(UNIX AND NOT APPLE)
    add_library(store STATIC ${INCLUDE_DIR}/store/store.c)
    target_include_directories(store PUBLIC 
        ${INCLUDE_DIR}
        ${LIBSECRET_INCLUDE_DIRS}
        ${GLIB_INCLUDE_DIRS}
    )
    target_link_libraries(store PUBLIC 
        ${LIBSECRET_LIBRARIES}
        ${GLIB_LIBRARIES}
    )
# SQLite implementation of keyring
elseif(APPLE)
    add_library(store STATIC ${INCLUDE_DIR}/store/store_sql.c)
    target_include_directories(store PUBLIC 
        ${INCLUDE_DIR}
        ${SQLite3_INCLUDE_DIRS}
    )
    target_link_libraries(store PUBLIC SQLite::SQLite3)
endif()

# Parser library
add_library(parser STATIC ${INCLUDE_DIR}/parser/parser.cpp)
target_include_directories(parser PUBLIC ${INCLUDE_DIR} ${XSIMD_INCLUDE_DIR})
target_link_libraries(parser PUBLIC store totp)

# Logger 
add_library(logger STATIC ${INCLUDE_DIR}/logger/logger.cpp)

# Link directories for Linux
if(UNIX AND NOT APPLE)
    link_directories(${LIBSECRET_LIBRARY_DIRS} ${GLIB_LIBRARY_DIRS})
endif()

# Clipboard
# Disable clip examples and tests
set(CLIP_EXAMPLES OFF CACHE BOOL "Compile clip examples")
set(CLIP_TESTS OFF CACHE BOOL "Compile clip tests")
#set(CLIP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/clip)

# Add clip subdirectory to compile the library
add_subdirectory(clip)

# Main executable
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/cli.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${INCLUDE_DIR}
    ${XSIMD_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
)
target_link_libraries(${PROJECT_NAME} PRIVATE parser OpenSSL::Crypto logger clip)

