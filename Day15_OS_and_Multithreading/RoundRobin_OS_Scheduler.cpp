#include <iostream>
#include <queue>
#include <string>

using namespace std;

struct Process {
    string name;
    int burst_time; // How much CPU time it needs to finish
};

int main() {
    cout << "--- DAY 15: OS ROUND ROBIN CPU SCHEDULING ---" << endl << endl;

    queue<Process> cpu_queue;
    cpu_queue.push({"ROS2_Navigation", 50});
    cpu_queue.push({"mmWave_Radar", 30});
    cpu_queue.push({"OLED_Display", 20});

    int time_quantum = 20; // Each process gets 20ms maximum per turn

    while (!cpu_queue.empty()) {
        Process current = cpu_queue.front();
        cpu_queue.pop();

        if (current.burst_time > time_quantum) {
            cout << "[OS] Executing " << current.name << " for " << time_quantum << "ms. (Remaining: "
                 << current.burst_time - time_quantum << "ms)" << endl;
            current.burst_time -= time_quantum;
            cpu_queue.push(current); // Put it back at the end of the line
        } else {
            cout << "[OS] Executing " << current.name << " for " << current.burst_time
                 << "ms. >>> TASK COMPLETED!" << endl;
        }
    }

    return 0;
}