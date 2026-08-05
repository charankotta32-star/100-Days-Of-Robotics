#include <iostream>
#include <cmath>

using namespace std;

class PIDController {
private:
    float kp, ki, kd;
    float previous_error = 0.0;
    float integral = 0.0;

public:
    PIDController(float p, float i, float d) : kp(p), ki(i), kd(d) {}

    float computeSteering(float target_position, float current_position) {
        float error = target_position - current_position;

        integral += error;
        float derivative = error - previous_error;

        // The PID Formula
        float steering_adjustment = (kp * error) + (ki * integral) + (kd * derivative);

        previous_error = error;
        return steering_adjustment;
    }
};

int main() {
    cout << "--- DAY 5: AUTONOMOUS PID STEERING ALGORITHM ---" << endl << endl;

    // Tuning the PID values (Proportional, Integral, Derivative)
    PIDController steeringPID(0.5, 0.01, 0.1);

    float track_center = 0.0; // We want the robot to stay perfectly at 0
    float robot_current_pos = 2.5; // Robot drifted 2.5 cm to the right!

    cout << "[SENSOR] Robot drifted to position: " << robot_current_pos << "cm" << endl;

    // Calculate how much the motors need to turn to fix it
    for(int i = 1; i <= 3; i++) {
        float correction = steeringPID.computeSteering(track_center, robot_current_pos);
        cout << "Iteration " << i << " | Applying Motor Correction: " << correction << " degrees" << endl;

        // Simulate the robot moving closer to the center
        robot_current_pos += correction;
    }

    return 0;
}