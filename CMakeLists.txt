cmake_minimum_required(VERSION 3.10)

project(TestCaseToolApp)

set(CMAKE_CXX_STANDARD 17)


# Server executable
add_executable(TestCaseToolApp
    src/main.cpp 
)


enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

