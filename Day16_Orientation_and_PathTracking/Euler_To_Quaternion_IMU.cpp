#include <iostream>
#include <cmath>

using namespace std;

// ROS 2 geometry_msgs::msg::Quaternion structure
struct Quaternion {
    double w, x, y, z;
};

class OrientationMath {
public:
    // Converts Euler angles (Roll, Pitch, Yaw in Radians) to a 4D Quaternion
    Quaternion eulerToQuaternion(double roll, double pitch, double yaw) {
        double cr = cos(roll * 0.5);
        double sr = sin(roll * 0.5);
        double cp = cos(pitch * 0.5);
        double sp = sin(pitch * 0.5);
        double cy = cos(yaw * 0.5);
        double sy = sin(yaw * 0.5);

        Quaternion q;
        q.w = cr * cp * cy + sr * sp * sy;
        q.x = sr * cp * cy - cr * sp * sy;
        q.y = cr * sp * cy + sr * cp * sy;
        q.z = cr * cp * sy - sr * sp * cy;

        return q;
    }
};

int main() {
    cout << "--- DAY 16: EULER TO QUATERNION CONVERSION (ROS 2 / ISAAC SIM) ---" << endl << endl;

    OrientationMath imuMath;

    // Robot rotated: Roll = 0 rad, Pitch = 0 rad, Yaw (Heading) = 90 degrees (1.5708 rad)
    double roll = 0.0;
    double pitch = 0.0;
    double yaw = M_PI / 2.0; // 90 degrees turn

    Quaternion q = imuMath.eulerToQuaternion(roll, pitch, yaw);

    cout << "Input Euler (Roll: " << roll << ", Pitch: " << pitch << ", Yaw: 90 deg)" << endl;
    cout << "ROS 2 Quaternion Vector [w, x, y, z]:" << endl;
    cout << " -> w: " << q.w << endl;
    cout << " -> x: " << q.x << endl;
    cout << " -> y: " << q.y << endl;
    cout << " -> z: " << q.z << endl << endl;

    return 0;
}