#include <iostream>
#include <string>

using namespace std;

struct TelemetryLog {
    float timestamp_sec;
    float radar_distance;
    TelemetryLog* next;
};

// Function to append node at TAIL
void insertAtTail(TelemetryLog** head_ref, float time, float dist) {
    TelemetryLog* new_node = new TelemetryLog{time, dist, nullptr};

    if (*head_ref == nullptr) {
        *head_ref = new_node;
        return;
    }

    TelemetryLog* temp = *head_ref;
    while (temp->next != nullptr) {
        temp = temp->next;
    }
    temp->next = new_node;
}

int main() {
    cout << "--- DAY 4: LINKED LIST TAIL INSERTION ---" << endl << endl;

    TelemetryLog* flight_log = nullptr;

    insertAtTail(&flight_log, 0.5, 4.2);
    insertAtTail(&flight_log, 1.0, 3.8);
    insertAtTail(&flight_log, 1.5, 3.1);

    cout << "Flight Log Stream (Chronological Order):" << endl;
    TelemetryLog* curr = flight_log;
    while (curr != nullptr) {
        cout << "T+" << curr->timestamp_sec << "s | Distance: " << curr->radar_distance << "m" << endl;
        curr = curr->next;
    }

    // Cleanup
    while (flight_log != nullptr) {
        TelemetryLog* temp = flight_log;
        flight_log = flight_log->next;
        delete temp;
    }

    return 0;
}