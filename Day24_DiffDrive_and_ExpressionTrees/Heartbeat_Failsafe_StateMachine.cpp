#include <iostream>
#include <chrono>
#include <thread>
#include <string>

using namespace std;

enum SystemState {
    STATE_OPTIMAL_OPERATION,
    STATE_WARNING_HEARTBEAT_DELAY,
    STATE_EMERGENCY_FAILSAFE_PARK
};

class HeartbeatMonitor {
private:
    chrono::steady_clock::time_point last_heartbeat_time;
    int warning_timeout_ms;
    int critical_timeout_ms;
    SystemState current_state;

public:
    HeartbeatMonitor(int warn_ms = 200, int crit_ms = 400)
        : warning_timeout_ms(warn_ms), critical_timeout_ms(crit_ms),
          current_state(STATE_OPTIMAL_OPERATION) {
        kick();
    }

    // Called whenever a valid telemetry packet arrives from high-level computer
    void kick() {
        last_heartbeat_time = chrono::steady_clock::now();
        if (current_state != STATE_OPTIMAL_OPERATION) {
            cout << "  ✅ [HEARTBEAT RESTORED] Resuming normal autonomous operations." << endl;
        }
        current_state = STATE_OPTIMAL_OPERATION;
    }

    void evaluateSafety() {
        auto now = chrono::steady_clock::now();
        auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(now - last_heartbeat_time).count();

        if (elapsed_ms >= critical_timeout_ms) {
            current_state = STATE_EMERGENCY_FAILSAFE_PARK;
            cout << "🚨 [CRITICAL WATCHDOG TIMEOUT] No heartbeat for " << elapsed_ms
                 << "ms! >>> EMERGENCY ACTUATOR CUT-OFF ENGAGED!" << endl;
        } else if (elapsed_ms >= warning_timeout_ms) {
            current_state = STATE_WARNING_HEARTBEAT_DELAY;
            cout << "⚠️ [HEARTBEAT WARNING] Delay of " << elapsed_ms << "ms detected. Throttling motor speeds." << endl;
        } else {
            cout << "🟢 [NOMINAL] Heartbeat Healthy. Delta: " << elapsed_ms << "ms / " << warning_timeout_ms << "ms." << endl;
        }
    }

    SystemState getState() const { return current_state; }
};

int main() {
    cout << "--- DAY 24: EMBEDDED HEARTBEAT FAILSAFE STATE MACHINE ---" << endl << endl;

    HeartbeatMonitor watchdog(150, 350); // Warning at 150ms, Emergency Park at 350ms

    // Step 1: Normal healthy communication
    cout << "Phase 1: Normal healthy telemetry stream (100ms intervals):" << endl;
    watchdog.evaluateSafety();
    this_thread::sleep_for(chrono::milliseconds(100));
    watchdog.kick();
    watchdog.evaluateSafety();

    // Step 2: Communication bus congestion (200ms delay)
    cout << "\nPhase 2: Bus congestion (200ms delay):" << endl;
    this_thread::sleep_for(chrono::milliseconds(200));
    watchdog.evaluateSafety(); // Warning state

    // Step 3: High-level computer freeze / disconnect (> 400ms)
    cout << "\nPhase 3: High-level OS freeze (450ms no signal):" << endl;
    this_thread::sleep_for(chrono::milliseconds(450));
    watchdog.evaluateSafety(); // Critical failsafe shutdown

    // Step 4: System recovery
    cout << "\nPhase 4: Telemetry bus recovered:" << endl;
    watchdog.kick();
    watchdog.evaluateSafety();

    return 0;
}