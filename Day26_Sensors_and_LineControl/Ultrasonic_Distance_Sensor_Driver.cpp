#include <iostream>
#include <iomanip>

using namespace std;

class UltrasonicSensor {
private:
    const double SPEED_OF_SOUND_CM_US = 0.0343; // Sound speed in air (cm/microsecond)
    double min_valid_cm;
    double max_valid_cm;

public:
    UltrasonicSensor(double min_cm = 2.0, double max_cm = 400.0)
        : min_valid_cm(min_cm), max_valid_cm(max_cm) {}

    // Converts echo pulse duration (microseconds) into obstacle distance (cm)
    double calculateDistance(uint32_t echo_time_us) {
        // Distance = (Time * Speed of Sound) / 2 (round trip)
        double distance_cm = (echo_time_us * SPEED_OF_SOUND_CM_US) / 2.0;

        // Out-of-range bounds safety clamping
        if (distance_cm < min_valid_cm || distance_cm > max_valid_cm) {
            return -1.0; // Invalid / Out of Range
        }
        return distance_cm;
    }
};

int main() {
    cout << "--- DAY 26: HC-SR04 ULTRASONIC RANGE SENSOR DRIVER ---" << endl << endl;

    UltrasonicSensor sonar(2.0, 400.0);
    cout << fixed << setprecision(1);

    // Simulated microsecond echo return times
    uint32_t test_pulses[] = { 583, 1166, 2915, 80 /* too close */, 35000 /* out of range */ };

    for (int i = 0; i < 5; i++) {
        uint32_t echo_us = test_pulses[i];
        double dist = sonar.calculateDistance(echo_us);

        cout << "Echo Pulse: " << setw(5) << echo_us << " us -> ";
        if (dist > 0) {
            cout << "Obstacle Distance: " << setw(5) << dist << " cm" << endl;
        } else {
            cout << "⚠️ Out of Sensor Range (Blind Zone or Far Horizon)" << endl;
        }
    }

    return 0;
}