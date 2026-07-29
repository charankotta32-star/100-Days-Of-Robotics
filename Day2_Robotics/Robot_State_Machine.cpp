#include <iostream>
#include <string>

using namespace std;

enum class SystemState {
    IDLE,
    SCANNING_WALL,
    TARGET_LOCKED,
    EMERGENCY_ALERT
};

class AegisFSM {
private:
    SystemState currentState;

public:
    AegisFSM() {
        currentState = SystemState::IDLE;
    }

    void transitionTo(SystemState newState) {
        currentState = newState;

        switch (currentState) {
            case SystemState::IDLE:
                cout << "[FSM] State: IDLE. Waiting for slap-on-wall trigger." << endl;
                break;
            case SystemState::SCANNING_WALL:
                cout << "[FSM] State: SCANNING. mmWave Radar emitting 24GHz pulse." << endl;
                break;
            case SystemState::TARGET_LOCKED:
                cout << "[FSM] State: TARGET LOCKED. Human breathing pattern confirmed at 0.2Hz." << endl;
                break;
            case SystemState::EMERGENCY_ALERT:
                cout << "[FSM] State: ALERT! Triggering Haptic Vibration & OLED Telemetry." << endl;
                break;
        }
    }
};

int main() {
    cout << "--- AEGIS BREACH-NODE STATE MACHINE ---" << endl << endl;

    AegisFSM robotBrain;

    // Simulate State Transitions
    robotBrain.transitionTo(SystemState::IDLE);
    robotBrain.transitionTo(SystemState::SCANNING_WALL);
    robotBrain.transitionTo(SystemState::TARGET_LOCKED);
    robotBrain.transitionTo(SystemState::EMERGENCY_ALERT);

    return 0;
}