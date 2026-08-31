#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

class NumericalIntegration {
public:
    // Trapezoidal Rule to find area under the curve (Distance = Area under Velocity curve)
    double calculateDistance(const vector<double>& velocity_samples, double dt) {
        if (velocity_samples.size() < 2) return 0.0;

        double total_distance = 0.0;

        for (size_t i = 1; i < velocity_samples.size(); i++) {
            // Area of a trapezoid = (a + b) / 2 * h
            double v_prev = velocity_samples[i - 1];
            double v_curr = velocity_samples[i];

            double trapezoid_area = ((v_prev + v_curr) / 2.0) * dt;
            total_distance += trapezoid_area;
        }

        return total_distance;
    }
};

int main() {
    cout << "--- DAY 31: MATHEMATICS (TRAPEZOIDAL RULE NUMERICAL INTEGRATION) ---" << endl << endl;

    NumericalIntegration mathEngine;
    cout << fixed << setprecision(3);

    // Rover velocity samples (m/s) taken every 0.1 seconds (dt = 0.1)
    double dt = 0.1;
    vector<double> v_samples = {0.0, 0.5, 1.2, 1.8, 1.8, 1.5, 0.8, 0.0};

    cout << "Time (s) | Velocity (m/s)" << endl;
    cout << "-----------------------" << endl;
    for (size_t i = 0; i < v_samples.size(); i++) {
        cout << setw(8) << (i * dt) << " | " << setw(8) << v_samples[i] << endl;
    }

    double distance = mathEngine.calculateDistance(v_samples, dt);

    cout << "\n>>> Total Distance Traveled (Area Under Curve): " << distance << " meters <<<" << endl;

    return 0;
}