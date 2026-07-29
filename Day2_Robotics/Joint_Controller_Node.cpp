#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

class JointControllerNode {
private:
    string target_joint;
    float current_angle;

public:
    JointControllerNode(string joint_name) {
        target_joint = joint_name;
        current_angle = 0.0;
        cout << "[SYSTEM] ROS2 Node Initialized. Target: " << target_joint << endl;
    }

    // The Publisher Function: Simulates sending a rotation command
    void publishMovement(float target_degrees) {
        cout << "[PUBLISHER] Sending command to " << target_joint << "..." << endl;

        // Simulating the physical time it takes for a heavy robot arm to move
        this_thread::sleep_for(chrono::milliseconds(800));

        current_angle = target_degrees;
        cout << ">>> SUCCESS: " << target_joint << " reached " << current_angle << " degrees." << endl << endl;
    }
};

int main() {
    cout << "--- OMNIVERSE KINEMATICS SIMULATION START ---" << endl << endl;

    // 1. We create the controller for the specific joint you saw in Isaac Sim
    JointControllerNode armController("panda_joint1");

    // 2. We command the robot to execute a sequence of movements
    armController.publishMovement(45.5);
    armController.publishMovement(90.0);
    armController.publishMovement(12.5);

    cout << "--- SIMULATION END ---" << endl;
    return 0;
}