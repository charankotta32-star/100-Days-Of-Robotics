#include <iostream>
#include <string>

using namespace std;

// Self-Referential Structure: Contains a pointer to another Node
struct TelemetryNode {
    int node_id;
    string sensor_type;
    float reading;
    TelemetryNode* next; // Pointer to the next node in the chain!
};

int main() {
    cout << "--- DAY 3: SELF-REFERENTIAL STRUCTURE CHAIN ---" << endl << endl;

    // Creating two static nodes
    TelemetryNode n1 = {101, "mmWave Radar", 3.5, nullptr};
    TelemetryNode n2 = {102, "LoRa Radio", 915.0, nullptr};

    // Linking Node 1 to Node 2
    n1.next = &n2;

    // Traversing the link using the Arrow Operator (->)
    cout << "Node 1 ID: " << n1.node_id << " (" << n1.sensor_type << ")" << endl;
    cout << "Linked Node 2 ID: " << n1.next->node_id << " (" << n1.next->sensor_type << ")" << endl;
    cout << "Linked Node 2 Reading: " << n1.next->reading << endl;

    return 0;
}