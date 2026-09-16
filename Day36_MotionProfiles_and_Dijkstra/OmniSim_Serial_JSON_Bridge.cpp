#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

struct SimulatedPose {
    double x;
    double y;
    double heading_deg;
    bool valid;
};

class OmniSimBridge {
public:
    // Outbound: Serializes motor commands into OmniSim-compliant JSON
    static string serializeCommand(double linear_v, double angular_w) {
        ostringstream oss;
        oss << fixed << setprecision(3);
        oss << "{\"wire_version\":\"1.0\",\"cmd_vel\":{\"linear\":"
            << linear_v << ",\"angular\":" << angular_w << "}}";
        return oss.str();
    }

    // Inbound: Parses raw simulated telemetry JSON returned by OmniSim
    static SimulatedPose parseTelemetry(const string& json_str) {
        SimulatedPose pose = {0.0, 0.0, 0.0, false};

        // Microcontroller safe string extraction (No heavy external JSON libs required)
        size_t pos_x = json_str.find("\"x\":");
        size_t pos_y = json_str.find("\"y\":");
        size_t pos_th = json_str.find("\"theta\":");

        if (pos_x != string::npos && pos_y != string::npos && pos_th != string::npos) {
            try {
                pose.x = stod(json_str.substr(pos_x + 4));
                pose.y = stod(json_str.substr(pos_y + 4));
                pose.heading_deg = stod(json_str.substr(pos_th + 8));
                pose.valid = true;
            } catch (...) {
                pose.valid = false;
            }
        }
        return pose;
    }
};

int main() {
    cout << "--- DAY 36: OMNISIM SERIAL/WIFI JSON WIRE PROTOCOL BRIDGE ---" << endl << endl;

    // 1. Outbound Transmission: ESP32-S3 formatting velocity command to OmniSim
    double commanded_v = 1.250;
    double commanded_w = -0.450;
    string wire_packet = OmniSimBridge::serializeCommand(commanded_v, commanded_w);

    cout << "1. Outbound ESP32-S3 Wire Packet (Transmitted to OmniSim):\n";
    cout << "   " << wire_packet << "\n\n";

    // 2. Inbound Reception: OmniSim returns simulated world coordinates back to ESP32
    string mock_sim_response = "{\"status\":\"ok\",\"pose\":{\"x\":1.4601,\"y\":0.3754,\"theta\":10.381}}";

    cout << "2. Inbound OmniSim Telemetry Packet Received:\n";
    cout << "   " << mock_sim_response << "\n\n";

    SimulatedPose received_state = OmniSimBridge::parseTelemetry(mock_sim_response);

    if (received_state.valid) {
        cout << fixed << setprecision(4);
        cout << "3. Parsed State into Microcontroller Memory:\n";
        cout << "   ✅ Pose X       : " << received_state.x << " m\n";
        cout << "   ✅ Pose Y       : " << received_state.y << " m\n";
        cout << "   ✅ Heading Angle: " << received_state.heading_deg << "°\n";
    } else {
        cout << "   🚨 [ERROR] Malformed Wire Packet!\n";
    }

    return 0;
}