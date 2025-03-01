cmake_minimum_required(VERSION 3.27)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project setup
project(deadbolt LANGUAGES CXX C)

# Include FetchContent for dependencies
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)


# Locate libsecret and GLib using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSECRET REQUIRED libsecret-1)
pkg_check_modules(GLIB REQUIRED glib-2.0)

# Include directories
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Backend library
add_library(backend STATIC ${INCLUDE_DIR}/filebackend/filebackend.cpp)
target_include_directories(backend PUBLIC ${INCLUDE_DIR})

# Parser library
add_library(parser STATIC ${INCLUDE_DIR}/parser/parser.cpp)
target_include_directories(parser PUBLIC ${INCLUDE_DIR})

# KeyAPI library (requires GLib and libsecret)
add_library(keyapi STATIC ${INCLUDE_DIR}/keyapi/keyapi.c)
target_include_directories(keyapi PUBLIC 
    ${INCLUDE_DIR}
    ${LIBSECRET_INCLUDE_DIRS} # Include libsecret headers
    ${GLIB_INCLUDE_DIRS}      # Include GLib headers
)
target_link_libraries(keyapi PUBLIC 
    ${LIBSECRET_LIBRARIES} # Link libsecret
    ${GLIB_LIBRARIES}      # Link GLib
)
# Main executable
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/cli.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${INCLUDE_DIR}
    ${LIBSECRET_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
)
link_directories(${LIBSECRET_LIBRARY_DIRS} ${GLIB_LIBRARY_DIRS})

# Link libraries to backend
target_link_libraries(backend PUBLIC parser keyapi)

# Link libraries to the main executable
target_link_libraries(${PROJECT_NAME} 
    PRIVATE tui
    PRIVATE backend
    PRIVATE ${LIBSECRET_LIBRARIES} # Link libsecret
    PRIVATE ${GLIB_LIBRARIES} # Link GLib
)

