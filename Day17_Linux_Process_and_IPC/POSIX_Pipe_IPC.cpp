#include <iostream>
#include <unistd.h>
#include <cstring>

using namespace std;

int main() {
    cout << "--- DAY 17: POSIX PIPE INTER-PROCESS COMMUNICATION ---" << endl << endl;

    int pipe_fd[2]; // pipe_fd[0] = Read End, pipe_fd[1] = Write End
    if (pipe(pipe_fd) == -1) {
        cerr << "Pipe creation failed!" << endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid == 0) {
        // Child Process: Sensor Producer
        close(pipe_fd[0]); // Close unused read end

        char telemetry[] = "TARGET_LOCKED: 3.5m";
        cout << "[CHILD SENSOR] Writing Telemetry to Pipe: " << telemetry << endl;
        write(pipe_fd[1], telemetry, strlen(telemetry) + 1);
        close(pipe_fd[1]);
    } else {
        // Parent Process: Navigation Consumer
        close(pipe_fd[1]); // Close unused write end

        char read_buffer[100];
        read(pipe_fd[0], read_buffer, sizeof(read_buffer));
        cout << "[PARENT NAV] Read Telemetry from Pipe: " << read_buffer << endl;
        close(pipe_fd[0]);
    }

    return 0;
}