#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>

using namespace std;

// Lock-Free Hardware Register in RAM
atomic<float> live_motor_rpm(0.0f);
atomic<bool> system_running(true);

// Thread 1: High-Speed Motor Driver writing telemetry (e.g., 1000Hz loop)
void motorEncoderThread() {
    float current_speed = 100.0f;
    while (system_running) {
        current_speed += 25.5f;
        live_motor_rpm.store(current_speed); // Thread-safe lock-free write
        this_thread::sleep_for(chrono::milliseconds(150));
    }
}

// Thread 2: Dashboard / Telemetry Logger reading live RPM
void dashboardLoggerThread() {
    for (int i = 1; i <= 4; i++) {
        this_thread::sleep_for(chrono::milliseconds(200));
        float rpm = live_motor_rpm.load(); // Thread-safe lock-free read
        cout << "[TELEMETRY STREAM " << i << "] Live Motor RPM: " << rpm << " (Lock-Free Read)" << endl;
    }
    system_running.store(false); // Signal shutdown
}

int main() {
    cout << "--- DAY 16: C++20 LOCK-FREE ATOMIC CONCURRENCY ---" << endl << endl;

    thread t_writer(motorEncoderThread);
    thread t_reader(dashboardLoggerThread);

    t_writer.join();
    t_reader.join();

    cout << "\n[SYSTEM] Lock-Free Telemetry Cycle Complete." << endl;
    return 0;
}