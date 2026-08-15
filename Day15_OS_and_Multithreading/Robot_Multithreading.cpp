#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

// Thread 1: The Vision System
void runCamera() {
    for (int i = 1; i <= 5; i++) {
        cout << "[CAMERA] Processing Frame " << i << "..." << endl;
        this_thread::sleep_for(chrono::milliseconds(400));
    }
}

// Thread 2: The Motor System
void runMotors() {
    for (int i = 1; i <= 5; i++) {
        cout << "[MOTORS] Driving Forward step " << i << "..." << endl;
        this_thread::sleep_for(chrono::milliseconds(400));
    }
}

int main() {
    cout << "--- DAY 15: C++ MULTITHREADING (OS CONCURRENCY) ---" << endl << endl;

    // Launching both systems to run AT THE EXACT SAME TIME
    thread t1(runCamera);
    thread t2(runMotors);

    // The main program waits for both threads to finish before shutting down
    t1.join();
    t2.join();

    cout << "\n[SYSTEM] Both threads completed successfully." << endl;
    return 0;
}