#include <iostream>
#include <cmath>

using namespace std;

struct RobotPose {
    double x;     // X coordinate in meters
    double y;     // Y coordinate in meters
    double theta; // Heading orientation in radians
};

class OdometryTracker {
private:
    RobotPose pose;
    double track_width = 0.20; // 20cm wheel-to-wheel distance

public:
    OdometryTracker() : pose{0.0, 0.0, 0.0} {}

    void update(double v_left, double v_right, double dt) {
        // Forward Kinematics: Linear velocity (v) and Angular velocity (w)
        double v = (v_right + v_left) / 2.0;
        double w = (v_right - v_left) / track_width;

        // Dead Reckoning Integration
        pose.x += v * cos(pose.theta) * dt;
        pose.y += v * sin(pose.theta) * dt;
        pose.theta += w * dt;

        cout << "[ODOMETRY] Position: (X=" << pose.x << "m, Y=" << pose.y
             << "m) | Heading: " << pose.theta * (180.0 / M_PI) << " deg" << endl;
    }
};

int main() {
    cout << "--- DAY 18: WHEEL ODOMETRY DEAD RECKONING ---" << endl << endl;

    OdometryTracker roverOdom;

    // Simulate 3 seconds of driving with right turn (v_left = 0.5 m/s, v_right = 0.3 m/s)
    double dt = 0.5; // 500ms time step
    for (int t = 1; t <= 6; t++) {
        cout << "T+" << (t * dt) << "s: ";
        roverOdom.update(0.5, 0.3, dt);
    }

    return 0;
}