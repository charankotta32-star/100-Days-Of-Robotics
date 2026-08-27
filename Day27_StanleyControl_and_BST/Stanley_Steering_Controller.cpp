#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

class StanleyController {
private:
    double k_gain;           // Cross-track control gain
    double max_steer_deg;    // Max physical steering angle (e.g., +/- 35 degrees)
    double softening_factor; // Epsilon to prevent division by zero at low speeds

public:
    StanleyController(double gain = 0.8, double max_deg = 35.0, double eps = 0.1)
        : k_gain(gain), max_steer_deg(max_deg), softening_factor(eps) {}

    // Computes steering correction in degrees
    double computeSteering(double heading_error_deg, double cross_track_error_m, double speed_m_s) {
        // Convert heading error to radians
        double heading_err_rad = heading_error_deg * (M_PI / 180.0);

        // Stanley cross-track correction: atan2(k * e, v + eps)
        double cross_track_term_rad = atan2(k_gain * cross_track_error_m, speed_m_s + softening_factor);

        // Total commanded steering angle
        double total_steer_rad = heading_err_rad + cross_track_term_rad;
        double total_steer_deg = total_steer_rad * (180.0 / M_PI);

        // Clamp to physical steering rack bounds
        double clamped_steer = max(-max_steer_deg, min(max_steer_deg, total_steer_deg));
        return clamped_steer;
    }
};

int main() {
    cout << "--- DAY 27: STANLEY AUTONOMOUS PATH-TRACKING CONTROLLER ---" << endl << endl;

    StanleyController stanley(1.2, 35.0); // Gain = 1.2, Max Steer = 35 deg
    cout << fixed << setprecision(2);

    // Rover traveling at 1.5 m/s encountering various path deviations
    double speed = 1.5; // m/s

    struct Scenario {
        double heading_err; // deg
        double cross_track; // meters (positive = drifted right of path)
        string description;
    };

    Scenario test_cases[] = {
        {  0.0,  0.00, "On Track & Aligned" },
        { 10.0,  0.00, "Heading Misaligned (+10 deg)" },
        {  0.0,  0.40, "Parallel Offset (0.4m Right of Centerline)" },
        { -8.0, -0.30, "Drifted Left & Heading Outward" },
        { 25.0,  1.20, "Severe Path Deviation (Requires Max Steer)" }
    };

    cout << "Scenario                      | Heading Err | Cross-Track | Speed | Commanded Steer" << endl;
    cout << "----------------------------------------------------------------------------------" << endl;

    for (const auto& test : test_cases) {
        double steer = stanley.computeSteering(test.heading_err, test.cross_track, speed);

        cout << left << setw(30) << test.description << " | "
             << right << setw(6) << test.heading_err << " deg | "
             << setw(6) << test.cross_track << " m  | "
             << setw(4) << speed << " | "
             << setw(7) << steer << " deg" << endl;
    }

    return 0;
}