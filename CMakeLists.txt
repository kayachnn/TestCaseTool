cmake_minimum_required(VERSION 3.10)

project(TestCaseTool)

set(CMAKE_CXX_STANDARD 11)

# Server executable
add_executable(TestCaseTool
    src/main.cpp
)


# Include directories
target_include_directories(TestCaseTool PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

