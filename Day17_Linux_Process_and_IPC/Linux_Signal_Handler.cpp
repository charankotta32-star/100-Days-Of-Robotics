#include <iostream>
#include <csignal>
#include <unistd.h>

using namespace std;

// Signal Callback function triggered on Ctrl+C (SIGINT)
void emergencyShutdownHandler(int signum) {
    cout << "\n\n>>> [OS SIGNAL " << signum << " RECEIVED] EMERGENCY BRAKE TRIGGERED!" << endl;
    cout << ">>> Cutting motor power and safely parking actuators..." << endl;
    exit(signum);
}

int main() {
    cout << "--- DAY 17: LINUX POSIX SIGNAL HANDLING (SIGINT) ---" << endl;
    cout << "Robot running. Press 'Ctrl + C' to test signal interception..." << endl << endl;

    // Register signal handler for SIGINT (Interrupt Signal)
    signal(SIGINT, emergencyShutdownHandler);

    int count = 1;
    while (count <= 3) {
        cout << "[ROBOT LOOP " << count << "] Autonomous navigation active..." << endl;
        sleep(1);
        count++;
    }

    cout << "\n[TEST COMPLETE] Loop finished normally without interrupt." << endl;
    return 0;
}