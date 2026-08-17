#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main() {
    cout << "--- DAY 17: LINUX PROCESS FORK (OS SYSTEM CALL) ---" << endl << endl;

    cout << "[PARENT] Master Robot Controller PID: " << getpid() << endl;

    // fork() creates a brand new child process
    pid_t pid = fork();

    if (pid < 0) {
        cerr << "[ERROR] Fork system call failed." << endl;
        return 1;
    } else if (pid == 0) {
        // Child Process
        cout << "[CHILD] Subsystem Process Spawned! PID: " << getpid()
             << " | Parent PID: " << getppid() << endl;
        cout << "[CHILD] Initializing mmWave Radar Sensor Subprocess..." << endl;
    } else {
        // Parent Process (pid contains child's PID)
        wait(NULL); // Wait for child process to finish
        cout << "[PARENT] Child process finished execution. Master controller continuing." << endl;
    }

    return 0;
}