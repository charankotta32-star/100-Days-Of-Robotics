#include <iostream>
#include <queue>
#include <string>

using namespace std;

struct RobotCommand {
    int priority_level; // Higher number = higher priority (10 = Emergency, 1 = Idle)
    string action_name;

    // Operator for std::priority_queue
    bool operator<(const RobotCommand& other) const {
        return priority_level < other.priority_level;
    }
};

int main() {
    cout << "--- DAY 18: ROBOT PRIORITY COMMAND QUEUE ---" << endl << endl;

    priority_queue<RobotCommand> command_stream;

    // Incoming commands arriving in random order
    command_stream.push({2, "Log Environmental Temperature"});
    command_stream.push({5, "Navigate to Waypoint Alpha"});
    command_stream.push({10, "CRITICAL: E-STOP OBSTACLE COLLISION"});
    command_stream.push({3, "Scan mmWave Radar Sector"});

    cout << "Executing Robot Commands in Strict Priority Order:" << endl;
    while (!command_stream.empty()) {
        RobotCommand cmd = command_stream.top();
        command_stream.pop();
        cout << " [PRIORITY " << cmd.priority_level << "] Executing: " << cmd.action_name << endl;
    }

    return 0;
}