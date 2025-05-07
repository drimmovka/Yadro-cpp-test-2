cmake_minimum_required(VERSION 3.10)

project(test VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(third_party/googletest)

include_directories(../include)

add_executable(${PROJECT_NAME} 
    test.cpp
    ../src/application.cpp
    ../src/client.cpp
    ../src/computer_club.cpp
    ../src/computer_club_manager.cpp
    ../src/event_body.cpp
    ../src/event.cpp
    ../src/event_parser.cpp
    ../src/exception.cpp
    ../src/input_parser.cpp
    ../src/output_writer.cpp
    ../src/table.cpp
    ../src/time24.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE gtest_main)