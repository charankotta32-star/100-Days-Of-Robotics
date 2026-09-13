#include <iostream>

using namespace std;

#define MAX 5

class CircularQueue {
private:
    int arr[MAX];
    int front, rear;

public:
    CircularQueue() {
        front = -1;
        rear = -1;
    }

    bool isFull() {
        // Queue is full if front is right ahead of rear, or front is 0 and rear is MAX-1
        return (front == (rear + 1) % MAX);
    }

    bool isEmpty() {
        return (front == -1);
    }

    void enqueue(int val) {
        if (isFull()) {
            cout << "⚠️ Queue Overflow! Cannot insert " << val << endl;
            return;
        }

        // If queue is empty, initialize front
        if (front == -1) front = 0;

        // Circular increment of rear
        rear = (rear + 1) % MAX;
        arr[rear] = val;
        cout << " -> Enqueued: " << val << " (Front: " << front << ", Rear: " << rear << ")" << endl;
    }

    void dequeue() {
        if (isEmpty()) {
            cout << "🚨 Queue Underflow! Nothing to delete." << endl;
            return;
        }

        int val = arr[front];
        cout << " <- Dequeued: " << val;

        // If only one element was left, reset queue
        if (front == rear) {
            front = -1;
            rear = -1;
            cout << " (Queue is now empty)" << endl;
        } else {
            // Circular increment of front
            front = (front + 1) % MAX;
            cout << " (Front: " << front << ", Rear: " << rear << ")" << endl;
        }
    }

    void display() {
        if (isEmpty()) {
            cout << "Queue: [ EMPTY ]\n" << endl;
            return;
        }

        cout << "Queue: [ ";
        int i = front;
        while (true) {
            cout << arr[i] << " ";
            if (i == rear) break;
            i = (i + 1) % MAX;
        }
        cout << "]\n" << endl;
    }
};

int main() {
    cout << "--- DAY 35: DSA (CIRCULAR QUEUE USING ARRAY) ---" << endl << endl;

    CircularQueue cq;

    // 1. Fill the queue
    cq.enqueue(10);
    cq.enqueue(20);
    cq.enqueue(30);
    cq.enqueue(40);
    cq.enqueue(50); // Queue is now full (MAX = 5)
    cq.display();

    // 2. Try to overflow
    cq.enqueue(60);

    // 3. Dequeue two elements
    cq.dequeue();
    cq.dequeue();
    cq.display();

    // 4. Enqueue to show circular wrap-around
    cout << ">>> Demonstrating Circular Wrap-Around:" << endl;
    cq.enqueue(60);
    cq.enqueue(70);
    cq.display();

    return 0;
}