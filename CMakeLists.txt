cmake_minimum_required(VERSION 3.10)

project(TestCaseToolApp)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -c -emit-llvm")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Server executable
add_executable(TestCaseToolApp
    src/main.cpp 
)


enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

