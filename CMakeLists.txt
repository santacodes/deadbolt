cmake_minimum_required(VERSION 3.30.2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(deadbolt CXX C)


add_library(
    backend STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/filebackend/filebackend.cpp
)


add_library(
    parser STATIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/parser.cpp
)

add_library(
    keyapi STATIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/keyapi/keyapi.c
)

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG main # Important: Specify a version or a commit hash here.
)
FetchContent_MakeAvailable(ftxui)

target_include_directories(backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/cli.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(backend parser keyapi)
target_link_libraries(${PROJECT_NAME} PRIVATE backend)
target_link_libraries(${PROJECT_NAME} PRIVATE parser)
target_link_libraries(${PROJECT_NAME} PRIVATE keyapi)
target_link_libraries(${PROJECT_NAME} PRIVATE ftxui::screen)
target_link_libraries(${PROJECT_NAME} PRIVATE ftxui::dom)
target_link_libraries(${PROJECT_NAME} PRIVATE ftxui::component)


