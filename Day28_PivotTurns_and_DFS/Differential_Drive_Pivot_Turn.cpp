#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

struct MotorCommand {
    double left_velocity_m_s;
    double right_velocity_m_s;
    double left_rpm;
    double right_rpm;
};

class PivotTurnController {
private:
    double wheel_radius; // 'r' in meters
    double track_width;  // 'L' in meters

public:
    PivotTurnController(double r, double l) : wheel_radius(r), track_width(l) {}

    // Computes target wheel velocities for an in-place zero-radius pivot turn
    MotorCommand computePivotSpeeds(double target_angular_velocity_rad_s) {
        // v = 0 (No forward linear motion)
        // v_left  = 0 - (w * L / 2)
        // v_right = 0 + (w * L / 2)

        double v_left  = -(target_angular_velocity_rad_s * track_width / 2.0);
        double v_right =  (target_angular_velocity_rad_s * track_width / 2.0);

        MotorCommand cmd;
        cmd.left_velocity_m_s  = v_left;
        cmd.right_velocity_m_s = v_right;

        // RPM = (V / (2 * PI * r)) * 60
        cmd.left_rpm  = (v_left / (2.0 * M_PI * wheel_radius)) * 60.0;
        cmd.right_rpm = (v_right / (2.0 * M_PI * wheel_radius)) * 60.0;

        return cmd;
    }
};

int main() {
    cout << "--- DAY 28: ZERO-RADIUS PIVOT TURN KINEMATICS ---" << endl << endl;

    // Rover parameters: Wheel radius = 5.5cm (0.055m), Track width = 24cm (0.24m)
    PivotTurnController rover(0.055, 0.24);
    cout << fixed << setprecision(2);

    // Target turns in radians per second
    double target_turns[] = {
        M_PI / 2.0,   // Rotate Left (CCW) at 90 deg/sec
        -M_PI / 2.0,  // Rotate Right (CW) at 90 deg/sec
        M_PI,         // Fast Rotate Left (CCW) at 180 deg/sec
        -M_PI         // Fast Rotate Right (CW) at 180 deg/sec
    };

    string actions[] = {"Turn Left (90 deg/s)", "Turn Right (90 deg/s)",
                        "Fast Left (180 deg/s)", "Fast Right (180 deg/s)"};

    cout << "Command Action           | Target Omega | L_Wheel (m/s) | R_Wheel (m/s) | L_RPM | R_RPM" << endl;
    cout << "---------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < 4; i++) {
        MotorCommand cmd = rover.computePivotSpeeds(target_turns[i]);

        cout << left << setw(24) << actions[i] << " | "
             << right << setw(8) << target_turns[i] << " r/s | "
             << setw(10) << cmd.left_velocity_m_s << "    | "
             << setw(10) << cmd.right_velocity_m_s << "    | "
             << setw(5) << cmd.left_rpm << " | "
             << setw(5) << cmd.right_rpm << endl;
    }

    return 0;
}