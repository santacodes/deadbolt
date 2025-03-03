cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project setup
project(deadbolt LANGUAGES CXX C)

# Locate libsecret and GLib using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSECRET REQUIRED libsecret-1)
pkg_check_modules(GLIB REQUIRED glib-2.0)

# Allow users to provide their own xsimd path
set(XSIMD_PATH "" CACHE PATH "Path to xsimd include directory")

# Fallback to local path if not set
if(NOT XSIMD_PATH)
    set(XSIMD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/xsimd/include)
endif()
# Include xsimd (header-only, so no linking required)

# Include directories
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# TOTP library
add_library(totp STATIC 
    ${INCLUDE_DIR}/totp/totp.cpp
)
target_include_directories(totp PUBLIC ${INCLUDE_DIR})

# Store library (for TOTP key storage with libsecret)
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
target_include_directories(parser PUBLIC ${INCLUDE_DIR})
target_link_libraries(parser PUBLIC store totp)

# Ensure we can find the compiled libraries
link_directories(${LIBSECRET_LIBRARY_DIRS} ${GLIB_LIBRARY_DIRS})

# Main executable
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/cli.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${INCLUDE_DIR}
    ${LIBSECRET_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${XSIMD_PATH} 
) 
target_link_libraries(${PROJECT_NAME} PRIVATE parser)

