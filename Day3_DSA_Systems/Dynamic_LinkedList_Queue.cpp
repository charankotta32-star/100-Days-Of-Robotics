#include <iostream>
#include <string>

using namespace std;

struct PacketNode {
    int packet_id;
    string payload;
    PacketNode* next;
};

int main() {
    cout << "--- DAY 3: DYNAMIC HEAP MEMORY BUFFER ---" << endl << endl;

    // Dynamically allocating Head node on Heap
    PacketNode* head = new PacketNode{1, "TARGET_LOCKED: 3.2m", nullptr};

    // Dynamically allocating Second node on Heap
    head->next = new PacketNode{2, "BREATHING_CONFIRMED: 0.2Hz", nullptr};

    // Printing the dynamic buffer chain
    PacketNode* temp = head;
    while (temp != nullptr) {
        cout << "[PACKET " << temp->packet_id << "] Payload: " << temp->payload << endl;
        temp = temp->next;
    }

    // CLEANUP: Freeing Heap Memory (Preventing Memory Leaks)
    PacketNode* nextNode = head->next;
    delete head;
    delete nextNode;

    cout << "\n[SYSTEM] Heap memory safely deallocated." << endl;
    return 0;
}