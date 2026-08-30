#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

int main() {
    cout << "--- DAY 30: HASH TABLE O(1) SENSOR REGISTRY (DSA UNIT 5) ---" << endl << endl;

    // std::unordered_map is C++'s implementation of a Hash Table
    // Key: I2C Address (Hex/Int) -> Value: Sensor Name
    unordered_map<int, string> sensor_registry;

    // 1. Insertion - O(1)
    sensor_registry[0x68] = "MPU6050_IMU";
    sensor_registry[0x3C] = "OLED_Display_0.96";
    sensor_registry[0x40] = "PCA9685_PWM_Driver";
    sensor_registry[0x76] = "BMP280_Barometer";

    cout << "[INIT] Registered " << sensor_registry.size() << " I2C devices in Hash Table.\n\n";

    // 2. Constant Time O(1) Lookups
    int target_address = 0x68;
    cout << "Searching for device at I2C Address 0x68..." << endl;

    if (sensor_registry.find(target_address) != sensor_registry.end()) {
        cout << " -> ✅ [FOUND in O(1)]: " << sensor_registry[target_address] << endl;
    } else {
        cout << " -> ❌ Device not found!" << endl;
    }

    target_address = 0x99; // Non-existent address
    cout << "\nSearching for device at I2C Address 0x99..." << endl;
    if (sensor_registry.find(target_address) != sensor_registry.end()) {
        cout << " -> ✅ [FOUND]: " << sensor_registry[target_address] << endl;
    } else {
        cout << " -> ❌ [NOT FOUND in O(1)]: Address is empty." << endl;
    }

    return 0;
}