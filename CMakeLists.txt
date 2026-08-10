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
                    Day5_Advanced_ROS2/PID_Motor_Controller.cpp
                    Day5_Advanced_ROS2/ROS2_Service_Server.cpp
                    Day6_Sensor_Fusion/MPU6050_Accel_Struct.cpp
                    Day6_Sensor_Fusion/Motion_Trigger_FSM.cpp
                    Day6_Sensor_Fusion/Telemetry_Stack_Buffer.cpp
                    Day7_DSA_Queue_and_Sensors/Queue_ADT_LinkedList.cpp
                    Day7_DSA_Queue_and_Sensors/ROS2_Message_Queue.cpp
                    Day7_DSA_Queue_and_Sensors/Sensor_Data_Cleaner.cpp
                    Day8_DSA_Trees_and_Robotics/Binary_Tree_Sensor_Hierarchy.cpp
                    Day8_DSA_Trees_and_Robotics/BST_Obstacle_Distance_Search.cpp
                    Day8_DSA_Trees_and_Robotics/Tree_Traversal_Inorder.cpp
                    Day9_DSA_Graph_and_Navigation/Graph_Adjacency_Matrix.cpp
                    Day9_DSA_Graph_and_Navigation/BFS_Robot_Pathfinding.cpp
                    Day9_DSA_Graph_and_Navigation/Dijkstra_LoRa_Routing.cpp
                    Day10_Robotics_SLAM_and_Vision/Occupancy_Grid_Map.cpp
                    Day10_Robotics_SLAM_and_Vision/Kalman_Filter_1D.cpp
                    Day10_Robotics_SLAM_and_Vision/Image_Color_Thresholding.cpp)
        endif()
    endif()
endforeach()