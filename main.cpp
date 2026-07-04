#include <iostream>
#include <string>

using namespace std;

// 1. THE BASE CLASS (The Master Blueprint)
class Sensor {
public:
    string sensorName;

    // 'virtual' is a magic keyword. It means "I expect my children to change this function"
    virtual void scanEnvironment() {
        cout << "[Generic Sensor] Scanning..." << endl;
    }
};

// 2. THE CHILD CLASS (Inherits from Sensor)
class mmWaveRadar : public Sensor {
public:
    // We 'override' the master function to do exactly what the Radar needs
    void scanEnvironment() override {
        cout << "[" << sensorName << "] PENETRATING CONCRETE... Human Breathing Detected at 4.2 Meters." << endl;
    }
};

// 3. ANOTHER CHILD CLASS
class LoRaRadio : public Sensor {
public:
    void scanEnvironment() override {
        cout << "[" << sensorName << "] MESH NETWORK ACTIVE... Transmitting data to Commander Tablet." << endl;
    }
};

class AcousticMic : public Sensor {
public:
    void scanEnvironment() override {
        cout << "[" << sensorName << "] Filtering fire noise... Human tapping heard!" << endl;
    }
};

int main() {
    // Let's create our objects!
    mmWaveRadar radar;
    radar.sensorName = "HLK-LD2410";

    LoRaRadio radioNode;
    radioNode.sensorName = "SX1278";

    AcousticMic acousticMicNode;
    acousticMicNode.sensorName = "Microphone";

    // Execute the systems
    cout << "--- RESQ-SANJEEVINI BOOT SEQUENCE ---" << endl;
    radar.scanEnvironment();
    radioNode.scanEnvironment();
    acousticMicNode.scanEnvironment();

    return 0;
}
