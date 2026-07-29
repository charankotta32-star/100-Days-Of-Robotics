cmake_minimum_required(VERSION 3.28)
project(100_Days_Of_Robotics)

set(CMAKE_CXX_STANDARD 20)

# Automatically scan for existing .cpp files in the project directory
file(GLOB_RECURSE EXISTING_CPP_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")

foreach(FILE_PATH ${EXISTING_CPP_FILES})
    # Ignore CLion's internal build directory
    if(NOT FILE_PATH MATCHES "^cmake-build-debug")
        # Extract target name (e.g. "Self_Referential_Node")
        get_filename_component(TARGET_NAME ${FILE_PATH} NAME_WE)

        # Create executable for THIS SINGLE FILE ONLY
        if(NOT TARGET ${TARGET_NAME})
            add_executable(${TARGET_NAME} ${FILE_PATH}
                    Day3_DSA_Systems/Double_Pointer_Memory.cpp)
        endif()
    endif()
endforeach()