#include <iostream>
#include <string>

using namespace std;

struct SensorLog {
    int log_id;
    SensorLog* next;
};

// Function using Double Pointer (SensorLog**) to insert a node at the HEAD
void insertAtHead(SensorLog** head_ref, int new_id) {
    // 1. Allocate new node on Heap
    SensorLog* new_node = new SensorLog();
    new_node->log_id = new_id;

    // 2. Link new node to current head
    new_node->next = *head_ref;

    // 3. Move the head pointer to point to the new node
    *head_ref = new_node;
}

int main() {
    cout << "--- DAY 3: DOUBLE POINTER (HEAD MODIFIER) ---" << endl << endl;

    SensorLog* head = nullptr; // Initially empty list

    // Inserting logs using Double Pointer pass-by-address
    insertAtHead(&head, 300);
    insertAtHead(&head, 200);
    insertAtHead(&head, 100);

    // Print list
    SensorLog* current = head;
    cout << "Sensor Log Stream: ";
    while (current != nullptr) {
        cout << "[" << current->log_id << "] -> ";
        current = current->next;
    }
    cout << "NULL" << endl;

    // Memory Cleanup
    while (head != nullptr) {
        SensorLog* temp = head;
        head = head->next;
        delete temp;
    }

    return 0;
}