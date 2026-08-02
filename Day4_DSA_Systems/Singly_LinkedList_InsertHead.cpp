#include <iostream>
#include <string>

using namespace std;

struct AlertNode {
    int alert_id;
    string message;
    AlertNode* next;
};

// Function to prepend node at HEAD in O(1) time
void insertAtHead(AlertNode** head_ref, int id, string msg) {
    AlertNode* new_node = new AlertNode{id, msg, *head_ref};
    *head_ref = new_node;
    cout << "[SYSTEM] Emergency Alert #" << id << " prepended to HEAD." << endl;
}

int main() {
    cout << "--- DAY 4: LINKED LIST HEAD INSERTION ---" << endl << endl;

    AlertNode* alert_stream = nullptr; // Empty list

    insertAtHead(&alert_stream, 101, "Low Battery Warning");
    insertAtHead(&alert_stream, 102, "Obstacle Detected");
    insertAtHead(&alert_stream, 103, "CRITICAL: Human Breathing Detected!");

    // Print alert stream
    cout << "\nPriority Alert Queue (HEAD -> TAIL):" << endl;
    AlertNode* current = alert_stream;
    while (current != nullptr) {
        cout << " -> [ID: " << current->alert_id << "] " << current->message << endl;
        current = current->next;
    }

    // Cleanup
    while (alert_stream != nullptr) {
        AlertNode* temp = alert_stream;
        alert_stream = alert_stream->next;
        delete temp;
    }

    return 0;
}