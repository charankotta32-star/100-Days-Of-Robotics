#include <iostream>
#include <string>

using namespace std;

struct WaypointNode {
    int waypoint_id;
    string zone_name;
    WaypointNode* prev;
    WaypointNode* next;
};

class MissionNavigator {
private:
    WaypointNode* head;
    WaypointNode* current;

public:
    MissionNavigator() : head(nullptr), current(nullptr) {}

    void addWaypoint(int id, string name) {
        WaypointNode* new_node = new WaypointNode{id, name, nullptr, nullptr};
        if (head == nullptr) {
            head = current = new_node;
        } else {
            WaypointNode* temp = head;
            while (temp->next != nullptr) temp = temp->next;
            temp->next = new_node;
            new_node->prev = temp;
        }
    }

    void advance() {
        if (current && current->next) {
            current = current->next;
            cout << " -> [NAV ADVANCE] Reached Waypoint #" << current->waypoint_id 
                 << " (" << current->zone_name << ")" << endl;
        } else {
            cout << " -> [NAV WARNING] Final destination reached." << endl;
        }
    }

    void backtrack() {
        if (current && current->prev) {
            current = current->prev;
            cout << " <- [NAV RETREAT] Backtracked to Waypoint #" << current->waypoint_id 
                 << " (" << current->zone_name << ")" << endl;
        } else {
            cout << " <- [NAV WARNING] Already at starting home base." << endl;
        }
    }

    ~MissionNavigator() {
        while (head) {
            WaypointNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    cout << "--- DAY 20: DOUBLY LINKED LIST MISSION NAVIGATOR (DSA UNIT 2) ---" << endl << endl;

    MissionNavigator roverRoute;
    roverRoute.addWaypoint(101, "Home Base (0,0)");
    roverRoute.addWaypoint(102, "Corridor Alpha");
    roverRoute.addWaypoint(103, "Target Search Room B");

    // Advance forward
    roverRoute.advance();
    roverRoute.advance();

    // Encounter obstacle -> Backtrack safely using prev pointer
    cout << "\n>>> [ALERT] Heavy debris ahead! Reversing navigation..." << endl;
    roverRoute.backtrack();

    return 0;
}