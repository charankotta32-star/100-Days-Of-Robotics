#include <iostream>
#include <queue>
#include <string>

using namespace std;

// Simulating ROS2 QoS Message Queue Depth
class ROS2MessageQueue {
private:
    size_t qos_depth;
    queue<string> msg_buffer;

public:
    ROS2MessageQueue(size_t depth) : qos_depth(depth) {}

    void pushMessage(string msg) {
        if (msg_buffer.size() >= qos_depth) {
            cout << "[QOS OVERFLOW] Buffer full! Dropping oldest message: " << msg_buffer.front() << endl;
            msg_buffer.pop(); // Drop oldest message (FIFO)
        }
        msg_buffer.push(msg);
        cout << "[ROS2 TOPIC] Pushed: " << msg << endl;
    }

    void processNext() {
        if (!msg_buffer.empty()) {
            cout << ">>> Executing Callback on: " << msg_buffer.front() << endl;
            msg_buffer.pop();
        }
    }
};

int main() {
    cout << "--- DAY 7: ROS2 QOS MESSAGE QUEUE BUFFER ---" << endl << endl;

    ROS2MessageQueue ros_topic(2); // Queue Depth = 2

    ros_topic.pushMessage("Frame 1: Clear");
    ros_topic.pushMessage("Frame 2: Clear");
    ros_topic.pushMessage("Frame 3: OBSTACLE DETECTED!"); // Triggers overflow drop!

    cout << endl;
    ros_topic.processNext();

    return 0;
}