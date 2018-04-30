cmake_minimum_required(VERSION 3.8)
project(proj1)

set(CMAKE_CXX_STANDARD 11)
include_directories(${PROJECT_SOURCE_DIR} "lib/" "lib/kernel/" "lib/user/" "devices/" "examples/" "filesys/" "threads/" "userprog/" "utils/")
file(GLOB SOURCES "*/*.c" "*/*/*.c" "*/*.h" "*/*/*.h" main.cpp)
add_executable(proj1 ${SOURCES})