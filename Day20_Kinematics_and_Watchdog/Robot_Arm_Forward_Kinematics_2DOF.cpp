#include <iostream>
#include <cmath>

using namespace std;

struct EndEffectorPose {
    double x;
    double y;
};

class PlanarArm2DOF {
private:
    double L1; // Length of Link 1 in meters
    double L2; // Length of Link 2 in meters

public:
    PlanarArm2DOF(double link1_len, double link2_len) : L1(link1_len), L2(link2_len) {}

    // Computes End-Effector (X, Y) from joint angles in degrees
    EndEffectorPose computeForwardKinematics(double theta1_deg, double theta2_deg) {
        // Convert degrees to radians
        double t1_rad = theta1_deg * (M_PI / 180.0);
        double t2_rad = theta2_deg * (M_PI / 180.0);

        EndEffectorPose pose;
        pose.x = (L1 * cos(t1_rad)) + (L2 * cos(t1_rad + t2_rad));
        pose.y = (L1 * sin(t1_rad)) + (L2 * sin(t1_rad + t2_rad));

        return pose;
    }
};

int main() {
    cout << "--- DAY 20: 2-DOF ROBOTIC ARM FORWARD KINEMATICS (21MMH021F) ---" << endl << endl;

    // Robot arm: Link 1 = 0.5m (50cm), Link 2 = 0.4m (40cm)
    PlanarArm2DOF arm(0.5, 0.4);

    cout << "1. Fully Extended (Theta1 = 0 deg, Theta2 = 0 deg):" << endl;
    EndEffectorPose p1 = arm.computeForwardKinematics(0.0, 0.0);
    cout << " -> End Effector: (X: " << p1.x << "m, Y: " << p1.y << "m)" << endl << endl;

    cout << "2. Right Angle Elbow (Theta1 = 45 deg, Theta2 = 90 deg):" << endl;
    EndEffectorPose p2 = arm.computeForwardKinematics(45.0, 90.0);
    cout << " -> End Effector: (X: " << p2.x << "m, Y: " << p2.y << "m)" << endl << endl;

    return 0;
}