#include <iostream>
#include <string>

using namespace std;

struct MotionNode {
    float accel_value;
    MotionNode* next;
};

// Stack ADT (LIFO - Last In First Out)
class TelemetryStack {
private:
    MotionNode* top;

public:
    TelemetryStack() : top(nullptr) {}

    void push(float val) {
        MotionNode* new_node = new MotionNode{val, top};
        top = new_node;
        cout << "[STACK PUSH] Saved Accel Reading: " << val << "g" << endl;
    }

    void pop() {
        if (top == nullptr) return;
        MotionNode* temp = top;
        top = top->next;
        cout << "[STACK POP] Removed Reading: " << temp->accel_value << "g" << endl;
        delete temp;
    }

    ~TelemetryStack() {
        while (top != nullptr) pop();
    }
};

int main() {
    cout << "--- DAY 6: LINKED STACK ADT (DSA UNIT 3) ---" << endl << endl;

    TelemetryStack motionLog;

    motionLog.push(1.02);
    motionLog.push(2.15);
    motionLog.push(0.98);

    cout << endl << "Clearing Stack Buffer (Most recent first):" << endl;
    motionLog.pop();
    motionLog.pop();

    return 0;
}