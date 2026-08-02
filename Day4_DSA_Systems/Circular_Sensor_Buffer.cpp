#include <iostream>

using namespace std;

class CircularBuffer {
private:
    static const int SIZE = 4;
    float buffer[SIZE];
    int head = 0;
    int count = 0;

public:
    void push(float value) {
        buffer[head] = value;
        head = (head + 1) % SIZE; // Modulo arithmetic for circular wrapping
        if (count < SIZE) count++;
        cout << "[RING BUFFER] Pushed: " << value << "m" << endl;
    }

    void printBuffer() {
        cout << "Current Ring Buffer State: [ ";
        for (int i = 0; i < count; i++) {
            cout << buffer[i] << " ";
        }
        cout << "]" << endl << endl;
    }
};

int main() {
    cout << "--- DAY 4: CIRCULAR RING BUFFER (ESP32 MEMORY) ---" << endl << endl;

    CircularBuffer radarMemory;

    radarMemory.push(5.0);
    radarMemory.push(4.2);
    radarMemory.push(3.8);
    radarMemory.push(3.1);
    radarMemory.printBuffer();

    // Overwriting oldest value (5.0) using modulo arithmetic
    cout << "--> Buffer Full. Pushing new reading..." << endl;
    radarMemory.push(2.5);
    radarMemory.printBuffer();

    return 0;
}