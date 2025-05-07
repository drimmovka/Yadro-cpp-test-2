cmake_minimum_required(VERSION 3.10)

project(test VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(third_party/googletest)

include_directories(../include)

file(GLOB SOURCES "../src/*.cpp")

add_executable(${PROJECT_NAME} 
    ${SOURCES}
    test.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE gtest_main)