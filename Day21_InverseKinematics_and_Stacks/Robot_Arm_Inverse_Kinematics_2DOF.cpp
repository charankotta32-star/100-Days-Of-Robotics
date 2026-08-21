#include <iostream>
#include <cmath>

using namespace std;

struct JointAngles {
    double theta1_deg;
    double theta2_deg;
    bool reachable;
};

class PlanarArmIK {
private:
    double L1; // Link 1 length (meters)
    double L2; // Link 2 length (meters)

public:
    PlanarArmIK(double link1, double link2) : L1(link1), L2(link2) {}

    // Computes Joint Angles (theta1, theta2) in degrees to reach target (target_x, target_y)
    JointAngles computeIK(double target_x, double target_y, bool elbow_up = false) {
        double r_sq = (target_x * target_x) + (target_y * target_y);
        double r = sqrt(r_sq);

        // 1. Workspace Reachability Check
        if (r > (L1 + L2) || r < fabs(L1 - L2)) {
            cout << "🚨 [IK ERROR] Target (" << target_x << "m, " << target_y
                 << "m) is OUTSIDE the arm's reachable workspace (Max Reach: "
                 << (L1 + L2) << "m)!" << endl;
            return {0.0, 0.0, false};
        }

        // 2. Law of Cosines for Theta 2
        double cos_theta2 = (r_sq - (L1 * L1) - (L2 * L2)) / (2.0 * L1 * L2);

        // Clamp for numerical floating-point safety [-1.0, 1.0]
        cos_theta2 = max(-1.0, min(1.0, cos_theta2));

        double sin_theta2 = sqrt(1.0 - (cos_theta2 * cos_theta2));
        if (elbow_up) {
            sin_theta2 = -sin_theta2; // Elbow-up configuration
        }
        double theta2_rad = atan2(sin_theta2, cos_theta2);

        // 3. Trigonometric Angle Solution for Theta 1
        double k1 = L1 + (L2 * cos_theta2);
        double k2 = L2 * sin_theta2;
        double theta1_rad = atan2(target_y, target_x) - atan2(k2, k1);

        // Convert Radians to Degrees
        JointAngles angles;
        angles.theta1_deg = theta1_rad * (180.0 / M_PI);
        angles.theta2_deg = theta2_rad * (180.0 / M_PI);
        angles.reachable = true;

        return angles;
    }
};

int main() {
    cout << "--- DAY 21: 2-DOF INVERSE KINEMATICS SOLVER (21MMH021F) ---" << endl << endl;

    // Robot arm: Link 1 = 0.5m (50cm), Link 2 = 0.4m (40cm)
    PlanarArmIK arm(0.5, 0.4);

    // Target 1: Coordinate (X = 0.4m, Y = 0.4m)
    cout << "Target 1: Coordinate (X = 0.4m, Y = 0.4m)" << endl;
    JointAngles ik1 = arm.computeIK(0.4, 0.4, false); // Elbow Down
    if (ik1.reachable) {
        cout << " -> Solved Joint 1 (Base):  " << ik1.theta1_deg << " deg" << endl;
        cout << " -> Solved Joint 2 (Elbow): " << ik1.theta2_deg << " deg" << endl << endl;
    }

    // Target 2: Coordinate (X = 0.0m, Y = 0.8m) - Pointing straight along Y-axis
    cout << "Target 2: Coordinate (X = 0.0m, Y = 0.8m)" << endl;
    JointAngles ik2 = arm.computeIK(0.0, 0.8, false);
    if (ik2.reachable) {
        cout << " -> Solved Joint 1 (Base):  " << ik2.theta1_deg << " deg" << endl;
        cout << " -> Solved Joint 2 (Elbow): " << ik2.theta2_deg << " deg" << endl << endl;
    }

    // Target 3: Unreachable coordinate (X = 1.0m, Y = 1.0m) -> Distance = 1.414m > Max 0.9m
    cout << "Target 3: Coordinate (X = 1.0m, Y = 1.0m)" << endl;
    arm.computeIK(1.0, 1.0);

    return 0;
}