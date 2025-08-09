cmake_minimum_required(VERSION 3.10)

project(Chatting_Server)
file(GLOB_RECURSE source CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hh)

add_executable(MAIN ${source})
target_include_directories(MAIN PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/header)
