cmake_minimum_required(VERSION 3.28.3)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(DEADBOLT CXX C)


add_library(
    backend STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/filebackend/filebackend.cpp
)

add_library(
    mysqlite STATIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/sqlite/sqlite3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/include/sqlite/shell.c
)

add_library(

    parser STATIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/parser.cpp


)

set_target_properties(mysqlite PROPERTIES LINKER_LANGUAGE C)

target_include_directories(backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/cli.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(backend mysqlite parser)
target_link_libraries(${PROJECT_NAME} backend)
target_link_libraries(${PROJECT_NAME} mysqlite)
target_link_libraries(${PROJECT_NAME} parser)
