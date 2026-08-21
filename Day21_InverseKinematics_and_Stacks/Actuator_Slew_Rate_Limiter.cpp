#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

class SlewRateLimiter {
private:
    double max_rate_of_change; // Maximum allowed change per second (e.g. 50.0 units/sec)
    double previous_output;
    bool initialized;

public:
    SlewRateLimiter(double rate_limit) : max_rate_of_change(rate_limit), previous_output(0.0), initialized(false) {}

    double filter(double commanded_input, double dt_sec) {
        if (!initialized) {
            previous_output = commanded_input;
            initialized = true;
            return previous_output;
        }

        // Maximum allowed delta for this time step: max_delta = rate * dt
        double max_delta = max_rate_of_change * dt_sec;
        double requested_delta = commanded_input - previous_output;

        // Clamp the rate of change
        if (requested_delta > max_delta) {
            previous_output += max_delta; // Ramp Up
        } else if (requested_delta < -max_delta) {
            previous_output -= max_delta; // Ramp Down
        } else {
            previous_output = commanded_input; // Safe step within limit
        }

        return previous_output;
    }
};

int main() {
    cout << "--- DAY 21: ACTUATOR SLEW-RATE LIMITER (MOTOR PROTECTION) ---" << endl << endl;

    // Max acceleration: 40% PWM speed change per second
    SlewRateLimiter motorLimiter(40.0);

    double dt = 0.25; // 250ms control loop interval (4 Hz)
    double step_command = 100.0; // Sudden full throttle jump from 0% to 100%

    cout << "Simulating sudden 0% -> 100% Full-Throttle Step Command:" << endl;
    cout << "Time (s) | Commanded PWM | Filtered Safe Output to Motor Driver" << endl;
    cout << "------------------------------------------------------------" << endl;

    for (int step = 1; step <= 12; step++) {
        double t = step * dt;
        double safe_pwm = motorLimiter.filter(step_command, dt);
        cout << " T+" << t << "s   |     " << step_command << "%     |     " << safe_pwm << "%" << endl;
    }

    return 0;
}