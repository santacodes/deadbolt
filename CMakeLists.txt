cmake_minimum_required(VERSION 3.25)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(DEADBOLT CXX)

# add_library(
#     backend 
#     include/filebackend/filebackend.hpp 
#     include/sqlite/sqlite3.h

# )

# add_executable(${PROJECT_NAME} cli.cpp)

# include_directories(../include)

add_library(
    backend STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/filebackend/filebackend.cpp
)

target_include_directories(backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/cli.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} backend )