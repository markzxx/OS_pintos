cmake_minimum_required(VERSION 3.8)
project(proj1)

set(CMAKE_CXX_STANDARD 11)
include_directories(${PROJECT_SOURCE_DIR} "pintos/src/lib/" "pintos/src/lib/kernel/" "pintos/src/lib/user/" "pintos/src/devices/" "pintos/src/examples/" "pintos/src/filesys/" "pintos/src/threads/" "pintos/src/userprog/" "pintos/src/utils/")
file(GLOB SOURCES "*/*.c" "*/*/*.c" "*/*.h" "*/*/*.h" main.cpp)

add_executable(proj1 ${SOURCES})