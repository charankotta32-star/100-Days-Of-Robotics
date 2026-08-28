#include <iostream>
#include <chrono>
#include <thread>
#include <string>

using namespace std;

class SoftwareWatchdog {
private:
    int timeout_ms;
    chrono::steady_clock::time_point last_pet_time;

public:
    SoftwareWatchdog(int timeout) : timeout_ms(timeout) {
        pet(); // Initialize timer
    }

    // "Petting" or resetting the watchdog
    void pet() {
        last_pet_time = chrono::steady_clock::now();
        cout << " 🟢 [WDT PET] Watchdog timer reset. System healthy." << endl;
    }

    // Checking if the system froze
    bool checkSystemHealth(const string& task_name) {
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - last_pet_time).count();

        if (elapsed >= timeout_ms) {
            cout << "\n🚨 [CRITICAL WATCHDOG TIMEOUT] Task '" << task_name
                 << "' hung for " << elapsed << "ms! (Limit: " << timeout_ms << "ms)" << endl;
            cout << ">>> TRIGGERING FORCED MCU HARD REBOOT! <<<" << endl;
            return false; // System dead
        }
        cout << " -> " << task_name << " executing nominally. Lag: " << elapsed << "ms." << endl;
        return true;
    }
};

int main() {
    cout << "--- DAY 28: EMBEDDED TASK WATCHDOG TIMER (WDT) ---" << endl << endl;

    SoftwareWatchdog wdt(300); // 300ms watchdog timeout

    cout << "Phase 1: Healthy Control Loop" << endl;
    for (int i = 1; i <= 3; i++) {
        wdt.checkSystemHealth("PID Motor Control");
        this_thread::sleep_for(chrono::milliseconds(100)); // Normal execution
        wdt.pet();
    }

    cout << "\nPhase 2: Sensor Failure (Simulating Infinite Wire-Wait Loop)" << endl;
    this_thread::sleep_for(chrono::milliseconds(400)); // Simulating a frozen sensor reading

    // Watchdog triggers on next check
    wdt.checkSystemHealth("I2C Sensor Read");

    return 0;
}