#include <iostream>
#include <string>

using namespace std;

template <typename T, int CAPACITY>
class CircularSensorQueue {
private:
    T buffer[CAPACITY];
    int front;
    int rear;
    int count;

public:
    CircularSensorQueue() : front(0), rear(-1), count(0) {}

    bool isFull() const {
        return count == CAPACITY;
    }

    bool isEmpty() const {
        return count == 0;
    }

    // O(1) Ingestion: Enqueue telemetry data (overwrites oldest on overflow if configured)
    bool enqueue(T data) {
        if (isFull()) {
            cout << "⚠️ [QUEUE OVERFLOW] Buffer full! Dropping oldest packet to maintain real-time telemetry." << endl;
            dequeue(); // Discard oldest sample
        }
        rear = (rear + 1) % CAPACITY;
        buffer[rear] = data;
        count++;
        return true;
    }

    // O(1) Processing: Dequeue oldest telemetry data
    T dequeue() {
        if (isEmpty()) {
            cout << "🚨 [QUEUE UNDERFLOW] Buffer is empty!" << endl;
            return T();
        }
        T data = buffer[front];
        front = (front + 1) % CAPACITY;
        count--;
        return data;
    }

    int size() const { return count; }

    void display() const {
        if (isEmpty()) {
            cout << "Buffer: [ EMPTY ]" << endl;
            return;
        }
        cout << "Live Buffer (Oldest -> Newest): [ ";
        for (int i = 0; i < count; i++) {
            int idx = (front + i) % CAPACITY;
            cout << buffer[idx] << " ";
        }
        cout << "] (Occupancy: " << count << "/" << CAPACITY << ")" << endl;
    }
};

int main() {
    cout << "--- DAY 22: CIRCULAR QUEUE SENSOR RING BUFFER (DSA UNIT 2) ---" << endl << endl;

    // Fixed 5-slot circular buffer for mmWave radar distance pings
    CircularSensorQueue<double, 5> radarRingBuffer;

    cout << "1. Enqueuing initial 4 radar distance samples (meters):" << endl;
    radarRingBuffer.enqueue(3.45);
    radarRingBuffer.enqueue(3.42);
    radarRingBuffer.enqueue(3.40);
    radarRingBuffer.enqueue(3.38);
    radarRingBuffer.display();

    cout << "\n2. Consuming oldest telemetry sample:" << endl;
    double processed = radarRingBuffer.dequeue();
    cout << " -> Dequeued: " << processed << "m" << endl;
    radarRingBuffer.display();

    cout << "\n3. Pushing 3 new samples to trigger circular wrap-around:" << endl;
    radarRingBuffer.enqueue(3.35);
    radarRingBuffer.enqueue(3.30);
    radarRingBuffer.enqueue(3.25); // Triggers wrap-around overwrite
    radarRingBuffer.display();

    return 0;
}