#include <iostream>

using namespace std;

class DifferentialKinematics {
private:
    float track_width_meters = 0.20; // 20cm wheel-to-wheel width

public:
    void computeWheelVelocities(float linear_v, float angular_w) {
        // Differential drive kinematic equations
        float v_left  = linear_v - (angular_w * track_width_meters / 2.0f);
        float v_right = linear_v + (angular_w * track_width_meters / 2.0f);

        cout << "[ROS 2 TWIST CMD] Linear: " << linear_v << " m/s | Angular: " << angular_w << " rad/s" << endl;
        cout << " -> Left Wheel Speed:  " << v_left << " m/s" << endl;
        cout << " -> Right Wheel Speed: " << v_right << " m/s" << endl << endl;
    }
};

int main() {
    cout << "--- DAY 11: DIFFERENTIAL DRIVE KINEMATICS (21MMH024F) ---" << endl << endl;

    DifferentialKinematics roverKinematics;

    cout << "1. Driving Straight Forward:" << endl;
    roverKinematics.computeWheelVelocities(0.5, 0.0);

    cout << "2. Turning Right while Moving Forward:" << endl;
    roverKinematics.computeWheelVelocities(0.5, 1.2);

    cout << "3. Zero-Radius Turn-in-Place:" << endl;
    roverKinematics.computeWheelVelocities(0.0, 2.0);

    return 0;
}