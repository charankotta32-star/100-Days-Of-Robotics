#include <iostream>
#include <string>

using namespace std;

struct QueueNode {
    int packet_id;
    string payload;
    QueueNode* next;
};

// Queue ADT (FIFO - First In First Out)
class LinkedQueue {
private:
    QueueNode *front, *rear;

public:
    LinkedQueue() : front(nullptr), rear(nullptr) {}

    // Enqueue: Add element to the REAR in O(1)
    void enqueue(int id, string data) {
        QueueNode* new_node = new QueueNode{id, data, nullptr};
        if (rear == nullptr) {
            front = rear = new_node;
        } else {
            rear->next = new_node;
            rear = new_node;
        }
        cout << "[ENQUEUE] Packet #" << id << " added to Queue Rear." << endl;
    }

    // Dequeue: Remove element from the FRONT in O(1)
    void dequeue() {
        if (front == nullptr) {
            cout << "[QUEUE EMPTY] Nothing to dequeue." << endl;
            return;
        }
        QueueNode* temp = front;
        cout << "[DEQUEUE] Processing Packet #" << temp->packet_id << ": " << temp->payload << endl;
        front = front->next;

        if (front == nullptr) rear = nullptr;
        delete temp;
    }

    ~LinkedQueue() {
        while (front != nullptr) dequeue();
    }
};

int main() {
    cout << "--- DAY 7: LINKED QUEUE ADT (DSA UNIT 3) ---" << endl << endl;

    LinkedQueue sensorQueue;

    sensorQueue.enqueue(101, "RADAR: Target at 3.2m");
    sensorQueue.enqueue(102, "ACCEL: Impact 0.99g");
    sensorQueue.enqueue(103, "LORA: Packet Relayed");

    cout << endl << "Processing Queue (FIFO Order):" << endl;
    sensorQueue.dequeue();
    sensorQueue.dequeue();

    return 0;
}