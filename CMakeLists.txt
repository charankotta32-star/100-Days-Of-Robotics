cmake_minimum_required(VERSION 3.28)
project(100_Days_Of_Robotics)

set(CMAKE_CXX_STANDARD 20)

# Recursively find all .cpp files
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")

foreach(src ${SOURCES})
    # Ignore cmake build directory
    if(NOT src MATCHES "^cmake-build-debug")
        # Extract filename without extension
        get_filename_component(target_name ${src} NAME_WE)

        # Create an executable target ONLY for this single file
        if(NOT TARGET ${target_name})
            add_executable(${target_name} ${src}
                    Day4_DSA_Systems/Singly_LinkedList_InsertTail.cpp)
        endif()
    endif()
endforeach()