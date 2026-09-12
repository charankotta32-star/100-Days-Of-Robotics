#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

class SensorVarianceEstimator {
private:
    int window_size;
    vector<double> window;
    int index;
    bool is_filled;
    double stationary_variance_threshold;

public:
    SensorVarianceEstimator(int size = 5, double threshold = 0.05)
        : window_size(size), window(size, 0.0), index(0),
          is_filled(false), stationary_variance_threshold(threshold) {}

    void addSample(double sample) {
        window[index] = sample;
        index = (index + 1) % window_size;
        if (index == 0) is_filled = true;
    }

    double calculateVariance() const {
        int count = is_filled ? window_size : index;
        if (count < 2) return 0.0;

        // 1. Mean (mu)
        double sum = 0.0;
        for (int i = 0; i < count; i++) sum += window[i];
        double mean = sum / count;

        // 2. Variance (sigma^2)
        double variance_sum = 0.0;
        for (int i = 0; i < count; i++) {
            variance_sum += (window[i] - mean) * (window[i] - mean);
        }

        return variance_sum / (count - 1);
    }

    bool isStationary() const {
        return calculateVariance() < stationary_variance_threshold;
    }
};

int main() {
    cout << "--- DAY 33: IMU ROLLING NOISE VARIANCE ESTIMATOR ---" << endl << endl;

    SensorVarianceEstimator imuChecker(5, 0.04);
    cout << fixed << setprecision(4);

    // Stream 1: Rover sitting still on lab table (Minor white noise around 0 deg/s)
    vector<double> stationary_stream = { 0.02, -0.01, 0.03, -0.02, 0.01, 0.02 };

    cout << "1. Evaluating Stationary Resting Phase (Benchtop):" << endl;
    for (double reading : stationary_stream) {
        imuChecker.addSample(reading);
        cout << "   Raw Reading: " << setw(7) << reading << " d/s | Variance: "
             << setw(7) << imuChecker.calculateVariance() << " | "
             << (imuChecker.isStationary() ? "🟢 [STATIONARY - SAFE TO CALIBRATE]" : "⚠️ [VIBRATION]") << endl;
    }

    // Stream 2: Someone bumps the rover or motors kick in (Vibration noise jumps)
    vector<double> vibration_stream = { 0.45, -0.80, 1.20, -1.10, 0.90 };

    cout << "\n2. Evaluating Disturbance / Motion Phase (Motors Running):" << endl;
    for (double reading : vibration_stream) {
        imuChecker.addSample(reading);
        cout << "   Raw Reading: " << setw(7) << reading << " d/s | Variance: "
             << setw(7) << imuChecker.calculateVariance() << " | "
             << (imuChecker.isStationary() ? "🟢 [STATIONARY]" : "🚨 [VIBRATION DETECTED - CALIBRATION PAUSED]") << endl;
    }

    return 0;
}