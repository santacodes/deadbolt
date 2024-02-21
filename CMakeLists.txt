cmake_minimum_required(VERSION 3.28.3)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(DEADBOLT CXX C)


add_library(
    backend STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/filebackend/filebackend.cpp
)


add_library(

    parser STATIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/parser.cpp


)


target_include_directories(backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/cli.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(backend parser)
target_link_libraries(${PROJECT_NAME} backend)
target_link_libraries(${PROJECT_NAME} parser)
