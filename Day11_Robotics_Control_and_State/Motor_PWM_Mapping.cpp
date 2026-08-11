#include <iostream>
#include <algorithm>

using namespace std;

class MotorDriver {
private:
    int min_pwm = 0;
    int max_pwm = 255; // 8-bit timer resolution

public:
    int calculatePWM(float speed_percentage) {
        // Clamp percentage between 0% and 100%
        speed_percentage = max(0.0f, min(100.0f, speed_percentage));

        // Map 0-100% to 0-255 PWM integer
        int pwm_output = (int)((speed_percentage / 100.0f) * max_pwm);
        return pwm_output;
    }
};

int main() {
    cout << "--- DAY 11: MOTOR PWM SPEED MAPPING (21MMH021F) ---" << endl << endl;

    MotorDriver left_motor;

    cout << "Speed 0%   -> PWM: " << left_motor.calculatePWM(0.0) << endl;
    cout << "Speed 50%  -> PWM: " << left_motor.calculatePWM(50.0) << endl;
    cout << "Speed 85%  -> PWM: " << left_motor.calculatePWM(85.0) << endl;
    cout << "Speed 120% -> PWM: " << left_motor.calculatePWM(120.0) << " (Clamped to Max)" << endl;

    return 0;
}