#include <iostream>
#include <cmath>
#include <string>

using namespace std;

// Struct to store 3-Axis Accelerometer Telemetry
struct Accel3D {
    float x_g;
    float y_g;
    float z_g;

    // Method to calculate total G-force magnitude
    float getMagnitude() {
        return sqrt(x_g * x_g + y_g * y_g + z_g * z_g);
    }
};

int main() {
    cout << "--- DAY 6: MPU6050 ACCELEROMETER CALIBRATION ---" << endl << endl;

    // Simulating raw G-force pings (1.0g = gravity on Earth)
    Accel3D stationary = {0.02, 0.01, 0.99};
    Accel3D wall_slap  = {2.85, 1.40, 3.10};

    cout << "Stationary Magnitude: " << stationary.getMagnitude() << "g (Expected ~1.0g)" << endl;
    cout << "Wall Impact Magnitude: " << wall_slap.getMagnitude() << "g (High Impact Detected!)" << endl;

    return 0;
}