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
                    Day15_OS_and_Multithreading/RoundRobin_OS_Scheduler.cpp
                    Day16_Orientation_and_PathTracking/Euler_To_Quaternion_IMU.cpp
                    Day16_Orientation_and_PathTracking/Pure_Pursuit_Controller.cpp
                    Day16_Orientation_and_PathTracking/Atomic_Telemetry_Buffer.cpp
                    Day17_Linux_Process_and_IPC/Linux_Process_Fork.cpp
                    Day17_Linux_Process_and_IPC/POSIX_Pipe_IPC.cpp
                    Day17_Linux_Process_and_IPC/Linux_Signal_Handler.cpp
                    Day18_Odometry_and_State_Estimation/Differential_Drive_Odometry.cpp
                    Day18_Odometry_and_State_Estimation/Exponential_Filter_IMU.cpp
                    Day18_Odometry_and_State_Estimation/Priority_Command_Queue.cpp
                    Day19_Navigation_and_Power_Systems/Potential_Field_Navigation.cpp
                    Day19_Navigation_and_Power_Systems/Sliding_Window_Peak_Detector.cpp
                    Day19_Navigation_and_Power_Systems/Battery_Management_System.cpp
                    Day20_Kinematics_and_Watchdog/Robot_Arm_Forward_Kinematics_2DOF.cpp
                    Day20_Kinematics_and_Watchdog/Doubly_LinkedList_Waypoint_Navigator.cpp
                    Day20_Kinematics_and_Watchdog/Watchdog_Timer_FailSafe.cpp
                    Day21_InverseKinematics_and_Stacks/Robot_Arm_Inverse_Kinematics_2DOF.cpp
                    Day21_InverseKinematics_and_Stacks/Stack_Infix_To_Postfix_Parser.cpp
                    Day21_InverseKinematics_and_Stacks/Actuator_Slew_Rate_Limiter.cpp
                    Day22_SensorFusion_and_Queues/IMU_Complementary_Filter_6DOF.cpp
                    Day22_SensorFusion_and_Queues/Circular_Queue_Telemetry_Buffer.cpp
                    Day22_SensorFusion_and_Queues/Motor_PID_AntiWindup_Controller.cpp
                    Day23_Raycasting_and_PacketCRC/Bresenham_Raycasting_GridMap.cpp
                    Day23_Raycasting_and_PacketCRC/MinHeap_Sensor_Timestamp_Sync.cpp
                    Day23_Raycasting_and_PacketCRC/LoRa_Packet_CRC8_Checksum.cpp
                    Day24_DiffDrive_and_ExpressionTrees/DiffDrive_Twist_To_Wheel_Kinematics.cpp
                    Day24_DiffDrive_and_ExpressionTrees/Binary_Expression_Tree_Evaluator.cpp
                    Day24_DiffDrive_and_ExpressionTrees/Heartbeat_Failsafe_StateMachine.cpp
                    Day25_Sensors_and_Deques/Analog_IR_Distance_Sensor_Calibrator.cpp
                    Day25_Sensors_and_Deques/Deque_Trajectory_Command_Buffer.cpp
                    Day25_Sensors_and_Deques/Motor_Deadband_Stiction_Compensator.cpp
                    Day26_Sensors_and_LineControl/Ultrasonic_Distance_Sensor_Driver.cpp
                    Day26_Sensors_and_LineControl/Stack_Undo_Navigation_History.cpp
                    Day26_Sensors_and_LineControl/Proportional_Line_Follower.cpp
                    Day27_StanleyControl_and_BST/Stanley_Steering_Controller.cpp
                    Day27_StanleyControl_and_BST/BST_Obstacle_Distance_Index.cpp
                    Day27_StanleyControl_and_BST/Median_Filter_Spike_Rejection.cpp
                    Day28_PivotTurns_and_DFS/Differential_Drive_Pivot_Turn.cpp
                    Day28_PivotTurns_and_DFS/DFS_Maze_Traversal.cpp
                    Day28_PivotTurns_and_DFS/Hardware_Task_Watchdog_Timer.cpp
                    Day29_Ackermann_and_BFS/BFS_Shortest_Path_Maze.cpp
                    Day29_Ackermann_and_BFS/Ackermann_Steering_Kinematics.cpp
                    Day29_Ackermann_and_BFS/Switch_Debounce_Filter.cpp
                    Day30_Transforms_and_Hashing/Coordinate_Transform_2D.cpp
                    Day30_Transforms_and_Hashing/Hash_Map_Sensor_Registry.cpp
                    Day30_Transforms_and_Hashing/Wheel_Encoder_Odometry.cpp
                    Day31_Math_and_Sorting/Trapezoidal_Rule_Odometry.cpp
                    Day31_Math_and_Sorting/Merge_Sort_Sensor_Data.cpp
                    Day31_Math_and_Sorting/PWM_Clock_Cycle_Math.cpp
            )
        endif()
    endif()
endforeach()