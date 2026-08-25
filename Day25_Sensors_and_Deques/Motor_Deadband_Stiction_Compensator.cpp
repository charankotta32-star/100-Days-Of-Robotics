#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

class DeadbandCompensator {
private:
    double min_effective_pwm; // Minimum PWM required to overcome motor static friction (e.g. 15%)
    double max_pwm;           // 100%

public:
    DeadbandCompensator(double min_pwm = 15.0, double max_p = 100.0)
        : min_effective_pwm(min_pwm), max_pwm(max_p) {}

    // Maps a raw PID command [-100%, 100%] into deadband-compensated motor output
    double compensate(double raw_command) {
        // If commanded to stop (0.0%), turn off motor completely
        if (fabs(raw_command) < 0.5) {
            return 0.0;
        }

        double sign = (raw_command > 0) ? 1.0 : -1.0;
        double abs_cmd = fabs(raw_command);

        // Linear interpolation from [min_effective_pwm, max_pwm]
        double compensated_pwm = min_effective_pwm + (abs_cmd / 100.0) * (max_pwm - min_effective_pwm);

        // Clamp to 100%
        compensated_pwm = min(max_pwm, compensated_pwm);

        return sign * compensated_pwm;
    }
};

int main() {
    cout << "--- DAY 25: DC MOTOR STICTION & DEADBAND COMPENSATOR ---" << endl << endl;

    // Minimum 15% PWM required to break static friction on physical rover wheels
    DeadbandCompensator motorDriver(15.0, 100.0);
    cout << fixed << setprecision(1);

    // Raw PID commands requesting very low to medium speeds
    double raw_pid_requests[] = { 0.0, 2.0, 8.0, 25.0, 50.0, -10.0 /* Reverse */ };

    cout << "Raw PID Output | Physical Motor State | Compensated PWM Sent to Driver" << endl;
    cout << "----------------------------------------------------------------------" << endl;

    for (int i = 0; i < 6; i++) {
        double raw = raw_pid_requests[i];
        double out = motorDriver.compensate(raw);

        cout << "    " << setw(5) << raw << "%     | ";
        if (raw == 0.0) cout << "Motor Stopped       |               " << setw(5) << out << "%" << endl;
        else if (fabs(raw) < 15.0) cout << "Deadband Broken!    |               " << setw(5) << out << "%" << endl;
        else cout << "Normal Drive Range  |               " << setw(5) << out << "%" << endl;
    }

    return 0;
}