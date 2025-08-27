cmake_minimum_required(VERSION 3.10)

project(Chatting_Server)
file(GLOB_RECURSE source CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hh)

if (NOT DEFINED MDB_FILE_PATH)
    set(MDB_FILE_PATH "/home/discopse/Documents/messages.json")
endif()
if (NOT DEFINED FILE_PATH)
    set(FILE_PATH "/home/discopse/Documents/clients.json")
endif()
if (NOT DEFINED PORT)
    set(PORT 8080)
endif()

file (WRITE ${CMAKE_CURRENT_SOURCE_DIR}/src/header/main.h "#define FILE_PATH \"${FILE_PATH}\" \n#define PORT ${PORT}\n#define MDB_FILE_PATH \"${MDB_FILE_PATH}\"")
find_package(RapidJSON REQUIRED)

add_executable(MAIN ${source})

target_include_directories(MAIN PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/header)
target_link_libraries(MAIN ${RapidJSON_LIBRARIES})