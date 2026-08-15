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
                    Day10_Robotics_SLAM_and_Vision/Image_Color_Thresholding.cpp
                    Day11_Robotics_Control_and_State/Motor_PWM_Mapping.cpp
                    Day11_Robotics_Control_and_State/Sensor_Calibration_Offset.cpp
                    Day11_Robotics_Control_and_State/Differential_Drive_Kinematics.cpp
                    Day12_Robotics_Computer_Vision/OpenCV_Frame_Difference.cpp
                    Day12_Robotics_Computer_Vision/Hash_Table_Spatial_Indexing.cpp
                    Day12_Robotics_Computer_Vision/Bounding_Box_IOU_Filter.cpp
                    Day13_Robotics_rPPG_and_Signals/Butterworth_Bandpass_Filter.cpp
                    Day13_Robotics_rPPG_and_Signals/Cross_Region_Correlation.cpp
                    Day13_Robotics_rPPG_and_Signals/QuickSort_Confidence_Scores.cpp
                    Day14_PathPlanning_and_Filtering/A_Star_Grid_Planner.cpp
                    Day14_PathPlanning_and_Filtering/Moving_Average_Filter.cpp
                    Day14_PathPlanning_and_Filtering/Trapezoidal_Velocity_Profile.cpp
                    Day15_OS_and_Multithreading/Robot_Multithreading.cpp
                    Day15_OS_and_Multithreading/Mutex_Data_Protection.cpp
                    Day15_OS_and_Multithreading/RoundRobin_OS_Scheduler.cpp)
        endif()
    endif()
endforeach()