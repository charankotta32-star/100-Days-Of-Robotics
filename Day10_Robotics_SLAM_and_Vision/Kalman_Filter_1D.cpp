#include <iostream>

using namespace std;

class KalmanFilter1D {
private:
    float estimate;           // Current position estimate
    float error_estimate;     // Uncertainty in estimate
    float error_measure;      // Sensor hardware noise

public:
    KalmanFilter1D(float init_est, float init_error_est, float sensor_noise)
        : estimate(init_est), error_estimate(init_error_est), error_measure(sensor_noise) {}

    float update(float measurement) {
        // 1. Calculate Kalman Gain
        float kalman_gain = error_estimate / (error_estimate + error_measure);

        // 2. Update state estimate with sensor measurement
        estimate = estimate + kalman_gain * (measurement - estimate);

        // 3. Update estimation error uncertainty
        error_estimate = (1.0 - kalman_gain) * error_estimate;

        return estimate;
    }
};

int main() {
    cout << "--- DAY 10: 1D KALMAN FILTER SENSOR FUSION ---" << endl << endl;

    // True robot distance is 10.0m. Sensor is noisy!
    KalmanFilter1D kf(0.0, 2.0, 0.8); // Initial guess 0m, high uncertainty, 0.8 sensor noise

    float noisy_pings[] = {9.2, 10.8, 9.5, 10.3, 10.1};

    cout << "Raw Noisy Ping  ->  Kalman Filtered Position" << endl;
    for (float ping : noisy_pings) {
        float filtered = kf.update(ping);
        cout << "  " << ping << "m         ->      " << filtered << "m" << endl;
    }

    return 0;
}