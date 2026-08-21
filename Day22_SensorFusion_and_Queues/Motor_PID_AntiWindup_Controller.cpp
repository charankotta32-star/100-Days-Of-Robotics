#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

class AntiWindupPID {
private:
    double Kp; // Proportional gain
    double Ki; // Integral gain
    double Kd; // Derivative gain

    double integral_sum;
    double prev_error;
    double output_min;
    double output_max;

public:
    AntiWindupPID(double p, double i, double d, double min_out, double max_out)
        : Kp(p), Ki(i), Kd(d), integral_sum(0.0), prev_error(0.0),
          output_min(min_out), output_max(max_out) {}

    double compute(double setpoint, double measured_value, double dt) {
        double error = setpoint - measured_value;

        // 1. Proportional Term
        double P_out = Kp * error;

        // 2. Derivative Term
        double derivative = (error - prev_error) / dt;
        double D_out = Kd * derivative;

        // 3. Integral Calculation with Anti-Windup Conditional Clamping
        double prospective_I = integral_sum + (Ki * error * dt);
        double prospective_output = P_out + prospective_I + D_out;

        // Anti-Windup: Only accumulate integral if actuator is NOT saturated
        if (prospective_output <= output_max && prospective_output >= output_min) {
            integral_sum = prospective_I;
        } else {
            // Actuator is saturated (e.g. stalled motor) -> freeze integral accumulation
            cout << " [ANTI-WINDUP ACTIVE] ";
        }

        // 4. Compute Final Clamped Output (0% to 100% PWM)
        double total_output = P_out + integral_sum + D_out;
        double clamped_output = max(output_min, min(output_max, total_output));

        prev_error = error;
        return clamped_output;
    }

    void reset() {
        integral_sum = 0.0;
        prev_error = 0.0;
    }
};

int main() {
    cout << "--- DAY 22: VELOCITY PID WITH INTEGRAL ANTI-WINDUP ---" << endl << endl;

    // PWM Output bounds: 0.0% to 100.0%
    AntiWindupPID pid(2.5, 1.2, 0.1, 0.0, 100.0);
    double target_speed = 50.0; // Target: 50 RPM
    double current_speed = 0.0;
    double dt = 0.1; // 100ms control loop

    cout << fixed << setprecision(1);
    cout << "Simulating a STALLED Motor (Obstacle contact prevents speed increase):" << endl;
    cout << "Step | Target RPM | Measured RPM | Commanded Motor Output" << endl;
    cout << "---------------------------------------------------------" << endl;

    for (int step = 1; step <= 8; step++) {
        // Motor is mechanically blocked: speed stays at 5 RPM despite high commanded PWM
        current_speed = (step <= 5) ? 5.0 : 45.0; // Cleared after step 5

        cout << " #" << step << "  |   "
             << setw(5) << target_speed << "    |    "
             << setw(5) << current_speed << "     |   ";

        double pwm = pid.compute(target_speed, current_speed, dt);
        cout << setw(5) << pwm << "% PWM" << endl;
    }

    cout << "\n>>> [STATUS] Actuator over-saturation prevented by Anti-Windup clamping." << endl;
    return 0;
}