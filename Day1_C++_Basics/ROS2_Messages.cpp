#include <iostream>
#include <string>

using namespace std;

struct RadarMessage {
    float distance_meters;
    int confidence_percent;
    bool human_detected;
};

class RescueNode {
private:
    string node_id = "ResQ-Sanjeevini-Alpha";
    int system_voltage = 5;

public:
    void processData(RadarMessage msg) {
        cout << "--- " << node_id << " INCOMING TRANSMISSION ---" << endl;

        if (msg.human_detected && msg.confidence_percent > 85) {
            cout << "[CRITICAL ALERT] Human breathing confirmed at " << msg.distance_meters << " meters!" << endl;
        } else {
            cout << "[INFO] Area clear or low confidence. Keep scanning." << endl;
        }
    }
};

int main() {
    cout << "BOOTING ROS2 MESSAGE SYSTEM..." << endl << endl;

    RescueNode commandCenter;

    RadarMessage sensor_data;
    sensor_data.distance_meters = 4.2;
    sensor_data.confidence_percent = 98; // 98% sure it's a human
    sensor_data.human_detected = true;

    commandCenter.processData(sensor_data);

    cout << endl << "--- SYSTEM SECURE. SHUTTING DOWN. ---" << endl;
    return 0;
}