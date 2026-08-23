#include <iostream>
#include <queue>
#include <vector>
#include <string>

using namespace std;

struct SensorPacket {
    uint64_t timestamp_ms; // Unix Epoch timestamp in milliseconds
    string sensor_name;
    double primary_reading;

    // Operator overload for Min-Heap (Lowest timestamp has highest priority)
    bool operator>(const SensorPacket& other) const {
        return timestamp_ms > other.timestamp_ms;
    }
};

class TemporalSensorSync {
private:
    priority_queue<SensorPacket, vector<SensorPacket>, greater<SensorPacket>> sync_queue;

public:
    void pushPacket(uint64_t ts, string name, double reading) {
        sync_queue.push({ts, name, reading});
        cout << "[INGEST ASYNC] " << name << " packet received with Timestamp: " << ts << "ms\n";
    }

    void processSynchronizedQueue() {
        cout << "\n--- PROCESSING CHRONOLOGICALLY SYNCHRONIZED STREAM ---\n";
        while (!sync_queue.empty()) {
            SensorPacket pkt = sync_queue.top();
            sync_queue.pop();
            cout << " -> [TIME: " << pkt.timestamp_ms << "ms] Sensor: "
                 << pkt.sensor_name << " | Reading: " << pkt.primary_reading << endl;
        }
    }
};

int main() {
    cout << "--- DAY 23: MIN-HEAP TEMPORAL SENSOR SYNCHRONIZER (DSA UNIT 3) ---" << endl << endl;

    TemporalSensorSync syncer;

    // Packets arriving out-of-order due to network/bus latency
    syncer.pushPacket(1724400000045ULL, "mmWave_Radar", 3.42);  // Arrived 1st, but timestamp is +45ms
    syncer.pushPacket(1724400000010ULL, "MPU6050_IMU", 0.98);   // Arrived 2nd, but timestamp is +10ms
    syncer.pushPacket(1724400000030ULL, "Wheel_Encoder", 45.0); // Arrived 3rd, timestamp +30ms
    syncer.pushPacket(1724400000000ULL, "Emergency_Bumper", 1.0); // Arrived 4th, earliest timestamp 0ms

    syncer.processSynchronizedQueue();

    return 0;
}