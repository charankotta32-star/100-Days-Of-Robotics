#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cstdint>

using namespace std;

enum MotorState {
    MOTOR_STOPPED,
    MOTOR_FORWARD,
    MOTOR_REVERSE,
    MOTOR_BRAKE
};

class BTS7960Driver {
private:
    string channel_name;
    int pwm_duty; // -100 to +100
    MotorState current_state;

public:
    BTS7960Driver(string name) : channel_name(name), pwm_duty(0), current_state(MOTOR_STOPPED) {}

    // Commanded speed: -100 (Full Reverse) to +100 (Full Forward)
    void setSpeed(int speed_percent) {
        // Clamp input to valid range
        int clamped_speed = max(-100, min(100, speed_percent));
        pwm_duty = clamped_speed;

        cout << "[" << channel_name << "] Command: " << setw(4) << speed_percent << "% | ";

        if (clamped_speed > 0) {
            // Forward: R_EN=1, L_EN=1, RPWM=duty, LPWM=0 (Shoot-Through Protected)
            current_state = MOTOR_FORWARD;
            cout << "PINS: R_EN=1, L_EN=1 | RPWM=" << setw(3) << clamped_speed
                 << "%, LPWM=0%   --> [FORWARD DRIVE]" << endl;
        } else if (clamped_speed < 0) {
            // Reverse: R_EN=1, L_EN=1, RPWM=0, LPWM=abs(duty)
            current_state = MOTOR_REVERSE;
            cout << "PINS: R_EN=1, L_EN=1 | RPWM=0%,   LPWM=" << setw(3) << abs(clamped_speed)
                 << "% --> [REVERSE DRIVE]" << endl;
        } else {
            // Coast Stop
            current_state = MOTOR_STOPPED;
            cout << "PINS: R_EN=0, L_EN=0 | RPWM=0%,   LPWM=0%   --> [COAST STOP]" << endl;
        }
    }

    // Active Dynamic Brake (Brings motor to immediate halt by shorting coils to ground)
    void activeBrake() {
        current_state = MOTOR_BRAKE;
        pwm_duty = 0;
        cout << "[" << channel_name << "] PINS: R_EN=1, L_EN=1 | RPWM=0%, LPWM=0% --> 🚨 [ACTIVE BRAKE ENGAGED]" << endl;
    }
};

int main() {
    cout << "--- DAY 32: BTS7960 43A DUAL H-BRIDGE DRIVER SIMULATOR ---" << endl << endl;

    BTS7960Driver leftTrack("LEFT_2x_MOTORS");
    BTS7960Driver rightTrack("RIGHT_2x_MOTORS");

    cout << "1. Commanded Full Forward Drive (300 RPM on 110mm Wheels):" << endl;
    leftTrack.setSpeed(80);
    rightTrack.setSpeed(80);

    cout << "\n2. Commanded Skid-Steer Pivot Turn (Left Reverse, Right Forward):" << endl;
    leftTrack.setSpeed(-70);
    rightTrack.setSpeed(70);

    cout << "\n3. Emergency Hazard Stop:" << endl;
    leftTrack.activeBrake();
    rightTrack.activeBrake();

    return 0;
}