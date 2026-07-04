#include <iostream>
using namespace std;

class Drone {
public:
    string name;

    Drone(string n) {
        name = n;
        cout << "[SYSTEM] Memory Allocated. Drone " << name << " created." << endl;
    }

    ~Drone() {
        cout << "[SYSTEM] Memory Freed. Drone " << name << " destroyed." << endl;
    }

    void fly() {
        cout << name << " is flying at 100 meters." << endl;
    }
};

int main() {
    cout << "--- INITIATING MANUAL MEMORY OVERRIDE ---" << endl;

    Drone* myDrone = new Drone("Sky-Node-1");
    myDrone->fly();
    delete myDrone;

    cout << endl;

    Drone* backupDrone = new Drone("Sky-Node-2");

    backupDrone->fly();

    delete backupDrone;
    cout << "--- OPERATION COMPLETE ---" << endl;
    return 0;
}