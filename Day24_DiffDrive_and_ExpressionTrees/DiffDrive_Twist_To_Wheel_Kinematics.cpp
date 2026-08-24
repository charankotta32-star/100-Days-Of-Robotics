#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

struct WheelSpeeds {
    double left_rad_s;
    double right_rad_s;
    double left_rpm;
    double right_rpm;
};

class DifferentialDriveKinematics {
private:
    double wheel_radius;    // Radius 'r' in meters (e.g. 0.05m = 5cm)
    double track_width;     // Baseline 'L' in meters (e.g. 0.22m = 22cm)
    double max_wheel_rad_s; // Max motor capability (e.g. 30.0 rad/s ~ 286 RPM)

public:
    DifferentialDriveKinematics(double r, double l, double max_rad_s)
        : wheel_radius(r), track_width(l), max_wheel_rad_s(max_rad_s) {}

    WheelSpeeds computeInverseKinematics(double linear_v, double angular_w) {
        // 1. Calculate linear speed at each wheel
        double v_left  = linear_v - (angular_w * track_width / 2.0);
        double v_right = linear_v + (angular_w * track_width / 2.0);

        // 2. Convert linear speed to angular wheel velocity (omega = v / r)
        double w_left  = v_left / wheel_radius;
        double w_right = v_right / wheel_radius;

        // 3. Proportional Saturation Scaling (Preserves Curvature Radius if Motors Saturate)
        double max_requested = max(fabs(w_left), fabs(w_right));
        if (max_requested > max_wheel_rad_s) {
            double scale = max_wheel_rad_s / max_requested;
            cout << "  ⚠️ [ACTUATOR SATURATION] Scaling wheel velocities by "
                 << (scale * 100.0) << "% to preserve turning trajectory." << endl;
            w_left *= scale;
            w_right *= scale;
        }

        WheelSpeeds speeds;
        speeds.left_rad_s  = w_left;
        speeds.right_rad_s = w_right;
        speeds.left_rpm    = w_left * (60.0 / (2.0 * M_PI));
        speeds.right_rpm   = w_right * (60.0 / (2.0 * M_PI));

        return speeds;
    }
};

int main() {
    cout << "--- DAY 24: DIFFERENTIAL DRIVE TWIST KINEMATICS (21MMH024F) ---" << endl << endl;

    // Robot params: Wheel radius = 5cm (0.05m), Track width = 22cm (0.22m), Max Speed = 25 rad/s (~238 RPM)
    DifferentialDriveKinematics rover(0.05, 0.22, 25.0);

    cout << fixed << setprecision(2);

    // Test Case 1: Straight line drive (v = 0.5 m/s, w = 0.0 rad/s)
    cout << "1. Command: Pure Forward Drive (v = 0.5 m/s, w = 0.0 rad/s)" << endl;
    WheelSpeeds ws1 = rover.computeInverseKinematics(0.5, 0.0);
    cout << "   -> Left Wheel:  " << setw(6) << ws1.left_rad_s << " rad/s (" << ws1.left_rpm << " RPM)" << endl;
    cout << "   -> Right Wheel: " << setw(6) << ws1.right_rad_s << " rad/s (" << ws1.right_rpm << " RPM)" << endl << endl;

    // Test Case 2: Zero-radius pivot turn (v = 0.0 m/s, w = 2.0 rad/s)
    cout << "2. Command: In-Place Zero-Radius Turn (v = 0.0 m/s, w = 2.0 rad/s)" << endl;
    WheelSpeeds ws2 = rover.computeInverseKinematics(0.0, 2.0);
    cout << "   -> Left Wheel:  " << setw(6) << ws2.left_rad_s << " rad/s (" << ws2.left_rpm << " RPM)" << endl;
    cout << "   -> Right Wheel: " << setw(6) << ws2.right_rad_s << " rad/s (" << ws2.right_rpm << " RPM)" << endl << endl;

    // Test Case 3: High-speed sharp curve triggering actuator saturation
    cout << "3. Command: High-Speed Sharp Curve (v = 1.2 m/s, w = 4.0 rad/s)" << endl;
    WheelSpeeds ws3 = rover.computeInverseKinematics(1.2, 4.0);
    cout << "   -> Clamped Left:  " << setw(6) << ws3.left_rad_s << " rad/s (" << ws3.left_rpm << " RPM)" << endl;
    cout << "   -> Clamped Right: " << setw(6) << ws3.right_rad_s << " rad/s (" << ws3.right_rpm << " RPM)" << endl << endl;

    return 0;
}