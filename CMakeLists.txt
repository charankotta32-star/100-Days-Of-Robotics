cmake_minimum_required(VERSION 3.28)
project(100_Days_Of_Robotics)

set(CMAKE_CXX_STANDARD 20)

add_executable(Main_File main.cpp)
add_executable(Radar_Code Day1_C++_Basics/Inheritance_Radar.cpp)
add_executable(Memory_Code Day1_C++_Basics/Dynamic_memory_management.cpp)