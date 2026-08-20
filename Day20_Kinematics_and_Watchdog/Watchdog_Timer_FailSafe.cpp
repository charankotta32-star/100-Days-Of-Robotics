#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

class SoftwareWatchdog {
private:
    chrono::steady_clock::time_point last_pet_time;
    int timeout_ms;

public:
    SoftwareWatchdog(int timeout) : timeout_ms(timeout) {
        pet();
    }

    // Resetting the watchdog timer ("kicking the dog")
    void pet() {
        last_pet_time = chrono::steady_clock::now();
        cout << "[WDT] Watchdog timer reset. System healthy." << endl;
    }

    // Checking if the main loop froze
    void checkStatus() {
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - last_pet_time).count();

        if (elapsed > timeout_ms) {
            cout << "🚨 [CRITICAL WATCHDOG TIMEOUT] Main loop froze for " << elapsed
                 << "ms! >>> TRIGGERING HARD SYSTEM REBOOT!" << endl;
        } else {
            cout << " -> Watchdog OK. Elapsed: " << elapsed << "ms / " << timeout_ms << "ms allowed." << endl;
        }
    }
};

int main() {
    cout << "--- DAY 20: EMBEDDED WATCHDOG TIMER (WDT) FAILSAFE ---" << endl << endl;

    SoftwareWatchdog wdt(500); // 500ms timeout threshold

    // Normal healthy loop
    wdt.checkStatus();
    this_thread::sleep_for(chrono::milliseconds(200));
    wdt.pet();

    // Simulating a frozen sensor or deadlock (> 500ms)
    cout << "\n>>> Simulating frozen sensor thread (600ms freeze)..." << endl;
    this_thread::sleep_for(chrono::milliseconds(600));

    wdt.checkStatus(); // Triggers watchdog timeout

    return 0;
}