#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

int shared_battery = 100; // Shared RAM resource
mutex battery_lock;       // The OS Lock (Mutual Exclusion)

void drainBattery(string system_name, int amount) {
    // LOCK the memory so no other thread can touch it
    battery_lock.lock();

    if (shared_battery >= amount) {
        cout << "[" << system_name << "] draining " << amount << "%..." << endl;
        shared_battery -= amount;
        cout << " -> Remaining Battery: " << shared_battery << "%" << endl;
    }

    // UNLOCK the memory so the next thread can use it
    battery_lock.unlock();
}

int main() {
    cout << "--- DAY 15: OS MUTEX & RESOURCE LOCKING ---" << endl << endl;

    // Both systems try to drain battery at the exact same time
    thread t1(drainBattery, "Radar Sensor", 40);
    thread t2(drainBattery, "Drive Motors", 50);

    t1.join();
    t2.join();

    cout << "\n[MAIN] Final Safe Battery Level: " << shared_battery << "%" << endl;
    return 0;
}