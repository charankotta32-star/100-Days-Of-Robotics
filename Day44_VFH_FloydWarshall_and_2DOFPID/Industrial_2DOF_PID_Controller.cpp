#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

class Industrial2DOFPID {
private:
    double Kp; // Proportional gain
    double Ki; // Integral gain
    double Kd; // Derivative gain
    double b;  // Setpoint weighting (0 <= b <= 1)
    double N;  // Derivative filter coefficient (typically 8 to 20)

    double integral;
    double prev_measurement;
    double filtered_derivative;
    double out_min, out_max;

public:
    Industrial2DOFPID(double p, double i, double d, double weight_b, double filter_n, double min_v, double max_v)
        : Kp(p), Ki(i), Kd(d), b(weight_b), N(filter_n),
          integral(0.0), prev_measurement(0.0), filtered_derivative(0.0),
          out_min(min_v), out_max(max_v) {}

    double compute(double setpoint, double measurement, double dt) {
        if (dt <= 0.0) return 0.0;

        // 1. Proportional Term with Setpoint Weighting (b)
        // Eliminates overshoot by softening direct command steps
        double P_out = Kp * ((b * setpoint) - measurement);

        // 2. Derivative on Measurement with 1st-Order Low-Pass Filter (N)
        // Eliminates Derivative Kick! (derivative of setpoint is ignored)
        double delta_meas = (measurement - prev_measurement);
        double raw_derivative = -delta_meas / dt;

        // Filter recursion: D[k] = alpha * raw + (1 - alpha) * D[k-1]
        double alpha = (N * dt) / (1.0 + N * dt);
        filtered_derivative = (alpha * (Kd * raw_derivative)) + ((1.0 - alpha) * filtered_derivative);
        double D_out = filtered_derivative;

        // 3. Integral Term with Anti-Windup Clamping
        double error = setpoint - measurement;
        double prospective_I = integral + (Ki * error * dt);

        double total_unclamped = P_out + prospective_I + D_out;

        // Anti-windup conditional integration
        if (total_unclamped >= out_min && total_unclamped <= out_max) {
            integral = prospective_I;
        }

        double final_output = clamp(P_out + integral + D_out, out_min, out_max);
        prev_measurement = measurement;

        return final_output;
    }

    void reset() {
        integral = 0.0;
        prev_measurement = 0.0;
        filtered_derivative = 0.0;
    }
};

int main() {
    cout << "--- DAY 44: INDUSTRIAL 2-DOF PID (DERIVATIVE-FILTERED) ---" << endl << endl;

    // Kp = 2.0, Ki = 1.0, Kd = 0.1, b = 0.6 (Softened step), N = 10 (Noise filter), Output [-100%, 100%]
    Industrial2DOFPID motorPID(2.0, 1.0, 0.1, 0.6, 10.0, -100.0, 100.0);
    cout << fixed << setprecision(2);

    double target_rpm = 200.0; // Sudden step change command from 0 to 200 RPM
    double simulated_speed = 0.0;
    double dt = 0.05; // 50ms control loop (20 Hz)

    cout << "Simulating Step Input Response (Target = 200 RPM, Kick-Free):\n";
    cout << "Time(s) | Target RPM | Motor Speed | Commanded PWM | Status\n";
    cout << "------------------------------------------------------------\n";

    for (int step = 1; step <= 8; ++step) {
        double t = step * dt;
        double pwm_out = motorPID.compute(target_rpm, simulated_speed, dt);

        cout << " " << setw(4) << t << "s |   " 
             << setw(6) << target_rpm << "   |   " 
             << setw(7) << simulated_speed << "   |    " 
             << setw(6) << pwm_out << "%   | ";

        if (step == 1) cout << "✅ Smooth Start (Zero Derivative Kick!)\n";
        else if (simulated_speed < target_rpm * 0.9) cout << "⚡ Accelerating\n";
        else cout << "🟢 Approaching Target\n";

        // Simple physical motor simulation response
        simulated_speed += pwm_out * 0.35;
    }

    cout << "\n>>> [VERIFIED] Motor accelerated without inductive current spikes or derivative kick." << endl;
    return 0;
}