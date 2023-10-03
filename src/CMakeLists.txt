cmake_minimum_required(VERSION 3.25)

project(DEADBOLT)

add_executable(${PROJECT_NAME} cli.cpp)

include_directories(../include)

