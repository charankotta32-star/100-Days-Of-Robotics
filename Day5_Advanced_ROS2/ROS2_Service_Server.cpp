#include <iostream>
#include <string>

using namespace std;

// Simulating the Request and Response packets of a ROS2 Service
struct CalibrationRequest {
    string command;
};

struct CalibrationResponse {
    bool success;
    string status_message;
};

class RadarServiceServer {
public:
    CalibrationResponse handleRequest(CalibrationRequest req) {
        CalibrationResponse res;

        cout << "[SERVICE SERVER] Received Request: " << req.command << endl;

        if (req.command == "CALIBRATE_MMWAVE") {
            cout << ">>> Executing hardware calibration sequence..." << endl;
            res.success = true;
            res.status_message = "Radar perfectly tuned to 0.2Hz (Human Lungs)";
        } else {
            res.success = false;
            res.status_message = "ERROR: Unknown command.";
        }

        return res;
    }
};

int main() {
    cout << "--- DAY 5: ROS 2 SERVICE (CLIENT/SERVER) ---" << endl << endl;

    RadarServiceServer mmWaveServer;

    // The Client sends a request
    CalibrationRequest myReq = {"CALIBRATE_MMWAVE"};

    // The Server processes it and sends back a response
    CalibrationResponse myRes = mmWaveServer.handleRequest(myReq);

    cout << "\n[CLIENT] Response Received -> Success: " << (myRes.success ? "True" : "False") << endl;
    cout << "[CLIENT] Message: " << myRes.status_message << endl;

    return 0;
}