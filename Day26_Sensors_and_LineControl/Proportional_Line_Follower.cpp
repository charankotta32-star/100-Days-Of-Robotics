#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

class LineTrackerP {
private:
    double Kp;        // Proportional steering gain
    double base_speed; // Base PWM speed (0% to 100%)

public:
    LineTrackerP(double p_gain, double base_pwm) : Kp(p_gain), base_speed(base_pwm) {}

    void computeMotorSpeeds(int sensor_error, double &left_pwm, double &right_pwm) {
        // Steering correction proportional to line position error
        double correction = Kp * sensor_error;

        left_pwm  = base_speed - correction;
        right_pwm = base_speed + correction;

        // Clamp within physical PWM bounds (0% to 100%)
        left_pwm  = max(0.0, min(100.0, left_pwm));
        right_pwm = max(0.0, min(100.0, right_pwm));
    }
};

int main() {
    cout << "--- DAY 26: 5-CHANNEL IR LINE TRACKER P-CONTROLLER ---" << endl << endl;

    LineTrackerP controller(15.0, 60.0); // Base speed 60%, Kp = 15.0
    cout << fixed << setprecision(1);

    // Simulated line positions: -2 (Hard Left), 0 (Centered), +2 (Hard Right)
    int line_errors[] = { 0, 1, 2, -1, -2 };
    string descriptions[] = { "Line Centered", "Slight Right Drift", "Sharp Right Curve",
                              "Slight Left Drift", "Sharp Left Curve" };

    cout << "Track Condition      | Error | Left Motor | Right Motor | Robot Action" << endl;
    cout << "------------------------------------------------------------------------" << endl;

    for (int i = 0; i < 5; i++) {
        double left, right;
        controller.computeMotorSpeeds(line_errors[i], left, right);

        cout << setw(20) << descriptions[i] << " | "
             << setw(5) << line_errors[i] << " | "
             << setw(7) << left << "%  | "
             << setw(8) << right << "%   | ";

        if (line_errors[i] == 0) cout << "Drive Straight" << endl;
        else if (line_errors[i] > 0) cout << "Steer Left" << endl;
        else cout << "Steer Right" << endl;
    }

    return 0;
}