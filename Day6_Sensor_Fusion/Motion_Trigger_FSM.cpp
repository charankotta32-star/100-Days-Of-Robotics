#include <iostream>

using namespace std;

enum class MotionState {
    MOVING_IN_HAND,
    WALL_CONTACT_STATIONARY,
    RADAR_SCANNING
};

class SlapScanTrigger {
private:
    MotionState state;

public:
    SlapScanTrigger() : state(MotionState::MOVING_IN_HAND) {}

    void processMotion(float accel_magnitude) {
        // If magnitude is close to 1.0g (gravity only), device is still!
        if (accel_magnitude >= 0.95 && accel_magnitude <= 1.05) {
            state = MotionState::WALL_CONTACT_STATIONARY;
            cout << "[MPU6050] Device Stationary on Wall. Waking up Radar..." << endl;
            state = MotionState::RADAR_SCANNING;
            cout << "[SYSTEM] >>> MMWAVE RADAR EMITTING 24GHz PULSE." << endl << endl;
        } else {
            state = MotionState::MOVING_IN_HAND;
            cout << "[MPU6050] Motion detected (" << accel_magnitude << "g). Radar Sleeping (IFF Guard Active)." << endl << endl;
        }
    }
};

int main() {
    cout << "--- DAY 6: ACCELEROMETER RADAR TRIGGER FSM ---" << endl << endl;

    SlapScanTrigger trigger;

    // 1. Walking down corridor (Sensor moving)
    trigger.processMotion(2.4);

    // 2. Slapped against wall (Sensor stationary)
    trigger.processMotion(0.99);

    return 0;
}