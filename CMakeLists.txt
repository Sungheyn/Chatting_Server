cmake_minimum_required(VERSION 3.10)

project(Chatting_Server)
file(GLOB_RECURSE source CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hh)

if (NOT DEFINED FILE_PATH)
    set(FILE_PATH "/home/discopse/Documents/clients.json")
endif()
if (NOT DEFINED PORT)
    set(PORT 8080)
endif()
file (WRITE ${CMAKE_CURRENT_SOURCE_DIR}/src/header/main.h "#define FILE_PATH \"${FILE_PATH}\" \n\r#define PORT ${PORT}")

add_executable(MAIN ${source})
target_include_directories(MAIN PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/header)