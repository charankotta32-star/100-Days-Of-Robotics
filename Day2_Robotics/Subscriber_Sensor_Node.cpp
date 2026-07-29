#include <iostream>
#include <string>

using namespace std;

// ROS2 Subscriber Node - Listens to mmWave Radar Distance
class RadarSubscriberNode {
private:
    float safety_threshold_meters = 0.8; // Stop if obstacle < 0.8m

public:
    // Callback function: Triggered automatically when new sensor data arrives
    void distanceCallback(float measured_distance) {
        cout << "[SUBSCRIBER] Incoming Radar Telemetry: " << measured_distance << "m" << endl;

        if (measured_distance < safety_threshold_meters) {
            cout << ">>> [EMERGENCY BRAKE] Obstacle too close! Halting motors immediately." << endl << endl;
        } else {
            cout << ">>> [STATUS] Path Clear. Continuing autonomous navigation." << endl << endl;
        }
    }
};

int main() {
    cout << "--- ROS2 RADAR SUBSCRIBER NODE ONLINE ---" << endl << endl;

    RadarSubscriberNode radarListener;

    // Simulating incoming radar streams
    radarListener.distanceCallback(3.2); // Safe
    radarListener.distanceCallback(1.5); // Safe
    radarListener.distanceCallback(0.4); // EMERGENCY STOP!

    return 0;
}