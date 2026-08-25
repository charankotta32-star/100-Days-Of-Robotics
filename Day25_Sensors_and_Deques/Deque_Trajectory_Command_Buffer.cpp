#include <iostream>
#include <string>

using namespace std;

class RobotCommandDeque {
private:
    string* arr;
    int front;
    int rear;
    int size;
    int capacity;

public:
    RobotCommandDeque(int cap = 6) : capacity(cap), front(-1), rear(0), size(0) {
        arr = new string[capacity];
    }

    bool isFull()  { return size == capacity; }
    bool isEmpty() { return size == 0; }

    // High-Priority Push: Inserts emergency commands directly to the FRONT of execution
    void pushFront(string cmd) {
        if (isFull()) {
            cout << "⚠️ Deque Overflow! Cannot push: " << cmd << endl;
            return;
        }
        if (front == -1) {
            front = 0;
            rear = 0;
        } else if (front == 0) {
            front = capacity - 1; // Wrap around to end
        } else {
            front--;
        }
        arr[front] = cmd;
        size++;
        cout << "🚨 [HIGH-PRIORITY INJECT] Pushed to Front: " << cmd << endl;
    }

    // Normal Push: Appends standard navigation waypoints to the REAR
    void pushRear(string cmd) {
        if (isFull()) {
            cout << "⚠️ Deque Overflow! Cannot push: " << cmd << endl;
            return;
        }
        if (front == -1) {
            front = 0;
            rear = 0;
        } else if (rear == capacity - 1) {
            rear = 0; // Wrap around to start
        } else {
            rear++;
        }
        arr[rear] = cmd;
        size++;
        cout << " -> [NORMAL QUEUE] Appended to Rear: " << cmd << endl;
    }

    // Executes the next command waiting at the front
    string popFront() {
        if (isEmpty()) {
            cout << "Buffer Empty!" << endl;
            return "";
        }
        string cmd = arr[front];
        if (front == rear) {
            front = -1;
            rear = -1;
        } else if (front == capacity - 1) {
            front = 0;
        } else {
            front++;
        }
        size--;
        return cmd;
    }

    ~RobotCommandDeque() {
        delete[] arr;
    }
};

int main() {
    cout << "--- DAY 25: DEQUE COMMAND BUFFER (DSA UNIT 2) ---" << endl << endl;

    RobotCommandDeque commandStream(5);

    // 1. Normal autonomous trajectory planned
    commandStream.pushRear("Move_Forward_1.0m");
    commandStream.pushRear("Rotate_Yaw_90deg");
    commandStream.pushRear("Scan_Sector_B");

    // 2. Sensor detects sudden hazard -> Injects emergency maneuver to FRONT
    cout << "\n>>> [HAZARD DETECTED] Overriding command queue:" << endl;
    commandStream.pushFront("EMERGENCY_STOP_BRAKE");

    // 3. Command execution order
    cout << "\n--- EXECUTING COMMAND STREAM ---" << endl;
    while (!commandStream.isEmpty()) {
        cout << " Executing: " << commandStream.popFront() << endl;
    }

    return 0;
}