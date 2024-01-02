cmake_minimum_required(VERSION 3.10)

project(TestCaseToolApp)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_OSX_ARCHITECTURES arm64)

# Server executable
add_executable(TestCaseToolApp
    src/main.cpp src/ClangAnalyseTool.cpp
)

add_executable(ClangAnalyseTool 
    src/ClangAnalyseTool.cpp)

find_package(LLVM REQUIRED CONFIG)
target_include_directories(ClangAnalyseTool PRIVATE 
    ${LLVM_INCLUDE_DIRS})

find_package(Clang REQUIRED CONFIG)
target_include_directories(ClangAnalyseTool PRIVATE
    ${CLANG_INCLUDE_DIRS})

# Add the Google Test framework
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Include directories
target_include_directories(TestCaseToolApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${LLVM_INCLUDE_DIRS}
)

