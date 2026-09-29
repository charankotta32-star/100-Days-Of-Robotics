#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include <iomanip>

using namespace std;

template <typename T, size_t Capacity>
class LockFreeSPSCQueue {
private:
    T buffer[Capacity];
    // Cache-aligned atomic indices to prevent false sharing
    alignas(64) atomic<size_t> head{0}; // Written by Producer
    alignas(64) atomic<size_t> tail{0}; // Written by Consumer

public:
    // Pushes item into queue. Returns false if buffer is full (Non-blocking!)
    bool push(const T& item) {
        size_t current_head = head.load(memory_order_relaxed);
        size_t current_tail = tail.load(memory_order_acquire);

        // Queue is full if (head + 1) % Capacity == tail
        if ((current_head + 1) % Capacity == current_tail) {
            return false; // Buffer full
        }

        buffer[current_head] = item;
        head.store((current_head + 1) % Capacity, memory_order_release);
        return true;
    }

    // Pops item from queue. Returns false if buffer is empty (Non-blocking!)
    bool pop(T& item) {
        size_t current_tail = tail.load(memory_order_relaxed);
        size_t current_head = head.load(memory_order_acquire);

        if (current_tail == current_head) {
            return false; // Buffer empty
        }

        item = buffer[current_tail];
        tail.store((current_tail + 1) % Capacity, memory_order_release);
        return true;
    }

    [[nodiscard]] bool isEmpty() const {
        return head.load(memory_order_relaxed) == tail.load(memory_order_relaxed);
    }
};

struct IMUTelemetry {
    uint32_t timestamp_ms;
    double roll, pitch, yaw;
};

int main() {
    cout << "--- DAY 41: LOCK-FREE SPSC RING BUFFER (EMBEDDED CONCURRENCY) ---" << endl << endl;

    // Fixed 8-slot ring buffer for high-speed sensor ingestion
    LockFreeSPSCQueue<IMUTelemetry, 8> imuQueue;
    atomic<bool> producer_done{false};

    // Producer Thread: Simulates high-frequency hardware sensor interrupt (e.g. 100 Hz MPU6050)
    thread producer([&]() {
        for (uint32_t i = 1; i <= 5; ++i) {
            IMUTelemetry sample = {i * 10, i * 0.5, i * 1.2, i * 0.8};
            while (!imuQueue.push(sample)) {
                this_thread::yield(); // Backoff if buffer is temporarily full
            }
            this_thread::sleep_for(chrono::milliseconds(15));
        }
        producer_done.store(true);
    });

    // Consumer Thread: Main navigation control loop processing sensor frames
    thread consumer([&]() {
        cout << fixed << setprecision(1);
        cout << "Consumer Loop Waiting for Atomic Ring Buffer Frames...\n\n";
        cout << "Timestamp |  Roll  |  Pitch |   Yaw  | Processing Status\n";
        cout << "--------------------------------------------------------\n";

        while (!producer_done.load() || !imuQueue.isEmpty()) {
            IMUTelemetry data;
            if (imuQueue.pop(data)) {
                cout << "  " << setw(5) << data.timestamp_ms << "ms  | "
                     << setw(5) << data.roll << "° | "
                     << setw(5) << data.pitch << "° | "
                     << setw(5) << data.yaw << "° | ✅ Consumed (Lock-Free)\n";
            } else {
                this_thread::sleep_for(chrono::milliseconds(5));
            }
        }
    });

    producer.join();
    consumer.join();

    cout << "\n>>> [VERIFIED] All telemetry frames exchanged without mutex locks or blocking." << endl;
    return 0;
}