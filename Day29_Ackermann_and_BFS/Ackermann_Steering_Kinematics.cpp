#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

class AckermannKinematics {
private:
    double wheelbase;  // Distance between front and rear axles (L)
    double track_width; // Distance between left and right wheels (T)

public:
    AckermannKinematics(double L, double T) : wheelbase(L), track_width(T) {}

    void computeSteeringAngles(double turning_radius_R) {
        if (turning_radius_R == 0) {
            cout << "Error: Turning radius cannot be zero for Ackermann steering!" << endl;
            return;
        }

        // Calculate inner and outer steering angles in radians
        double delta_inner = atan(wheelbase / (turning_radius_R - (track_width / 2.0)));
        double delta_outer = atan(wheelbase / (turning_radius_R + (track_width / 2.0)));

        // Convert to Degrees
        double inner_deg = delta_inner * (180.0 / M_PI);
        double outer_deg = delta_outer * (180.0 / M_PI);

        cout << "Turning Radius: " << turning_radius_R << " m" << endl;
        cout << " -> Inner Wheel Angle: " << setw(5) << inner_deg << " degrees" << endl;
        cout << " -> Outer Wheel Angle: " << setw(5) << outer_deg << " degrees" << endl;
        cout << " -> Difference (Ackermann Effect): " << (inner_deg - outer_deg) << " degrees" << endl << endl;
    }
};

int main() {
    cout << "--- DAY 29: ACKERMANN STEERING KINEMATICS ---" << endl << endl;

    // Rover parameters: Wheelbase (L) = 30cm, Track Width (T) = 20cm
    AckermannKinematics car_rover(0.30, 0.20);
    cout << fixed << setprecision(2);

    cout << "Simulating Right Turns of varying tightness:" << endl << endl;

    car_rover.computeSteeringAngles(2.0);  // Gentle turn
    car_rover.computeSteeringAngles(1.0);  // Moderate turn
    car_rover.computeSteeringAngles(0.4);  // Extremely sharp turn

    return 0;
}