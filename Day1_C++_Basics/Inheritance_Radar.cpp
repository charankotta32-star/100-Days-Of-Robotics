#include <iostream>
#include <string>

using namespace std;

class Sensor {
public:
    string sensorName;

    virtual void scanEnvironment() {
        cout << "[Generic Sensor] Scanning..." << endl;
    }
};

class mmWaveRadar : public Sensor {
public:
    void scanEnvironment() override {
        cout << "[" << sensorName << "] PENETRATING CONCRETE... Human Breathing Detected at 4.2 Meters." << endl;
    }
};

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

    mmWaveRadar radar;
    radar.sensorName = "HLK-LD2410";

    LoRaRadio radioNode;
    radioNode.sensorName = "SX1278";

    AcousticMic acousticMicNode;
    acousticMicNode.sensorName = "Microphone";

    cout << "--- RESQ-SANJEEVINI BOOT SEQUENCE ---" << endl;
    radar.scanEnvironment();
    radioNode.scanEnvironment();
    acousticMicNode.scanEnvironment();

    return 0;
}