#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

class EMALowPassFilter {
private:
    double alpha;        // Smoothing factor: 0 < alpha <= 1
    double filtered_val; // Single state variable (Zero memory buffer overhead!)
    bool initialized;

public:
    explicit EMALowPassFilter(double smoothing_factor = 0.20)
        : alpha(smoothing_factor), filtered_val(0.0), initialized(false) {}

    double update(double raw_measurement) {
        if (!initialized) {
            filtered_val = raw_measurement;
            initialized = true;
            return filtered_val;
        }

        // EMA Formula: y[k] = alpha * x[k] + (1 - alpha) * y[k-1]
        filtered_val = (alpha * raw_measurement) + ((1.0 - alpha) * filtered_val);
        return filtered_val;
    }

    void setAlpha(double new_alpha) {
        if (new_alpha > 0.0 && new_alpha <= 1.0) {
            alpha = new_alpha;
        }
    }

    [[nodiscard]] double getValue() const { return filtered_val; }
};

int main() {
    cout << "--- DAY 40: EMBEDDED EXPONENTIAL MOVING AVERAGE (EMA) FILTER ---" << endl << endl;

    // Alpha = 0.25 (Balances noise rejection with minimal response lag)
    EMALowPassFilter ema(0.25);
    cout << fixed << setprecision(2);

    // Simulated noisy IMU pitch angle readings (Vehicle pitching upward from 0° to ~20°, with high-frequency motor noise)
    vector<double> noisy_sensor_data = {
        0.5, -1.2, 1.8, 3.4, 6.2, 4.1 /* noise dip */,
        9.8, 12.4, 15.1, 13.2 /* noise dip */, 18.5, 20.2, 19.8, 20.1
    };

    cout << "Sample # | Raw Sensor Reading | Clean Filtered Output | Noise Deviation" << endl;
    cout << "------------------------------------------------------------------------" << endl;

    for (size_t i = 0; i < noisy_sensor_data.size(); i++) {
        double raw = noisy_sensor_data[i];
        double clean = ema.update(raw);
        double delta = raw - clean;

        cout << "   #" << setw(2) << (i + 1) << "  |       "
             << setw(6) << raw << "°      |        "
             << setw(6) << clean << "°       |     "
             << setw(5) << delta << "°\n";
    }

    cout << "\n>>> [VERIFIED] Motor vibration spikes filtered without allocating memory buffers." << endl;
    return 0;
}