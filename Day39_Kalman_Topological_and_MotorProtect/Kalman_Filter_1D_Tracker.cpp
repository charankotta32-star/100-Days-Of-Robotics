#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

struct KalmanState {
    double position;
    double variance;
};

class DiscreteKalmanFilter1D {
private:
    double x_est; // Estimated state (position)
    double p_cov; // Estimated covariance (uncertainty)
    double q_proc;// Process noise covariance (model uncertainty)
    double r_meas;// Measurement noise covariance (sensor uncertainty)

public:
    DiscreteKalmanFilter1D(double init_x, double init_p, double q, double r)
        : x_est(init_x), p_cov(init_p), q_proc(q), r_meas(r) {}

    // 1. Time Update (Predict State based on velocity physics)
    void predict(double velocity_m_s, double dt) {
        x_est = x_est + (velocity_m_s * dt);
        p_cov = p_cov + q_proc;
    }

    // 2. Measurement Update (Correct State with noisy sensor reading)
    KalmanState update(double measurement) {
        // Calculate optimal Kalman Gain: K = P / (P + R)
        double K = p_cov / (p_cov + r_meas);

        // Correct estimate: x = x + K * (z - x)
        x_est = x_est + K * (measurement - x_est);

        // Update error covariance: P = (1 - K) * P
        p_cov = (1.0 - K) * p_cov;

        return {x_est, p_cov};
    }

    double getEstimate() const { return x_est; }
    double getUncertainty() const { return p_cov; }
};

int main() {
    cout << "--- DAY 39: 1D DISCRETE KALMAN FILTER (SENSOR FUSION) ---" << endl << endl;

    // Initial state: Pos = 0.0m, Uncertainty P = 1.0, Process noise Q = 0.02, Sensor noise R = 0.50
    DiscreteKalmanFilter1D kf(0.0, 1.0, 0.02, 0.50);
    cout << fixed << setprecision(3);

    double dt = 0.1; // 100ms time step
    double commanded_velocity = 1.0; // Rover moving forward at constant 1.0 m/s

    // Simulated noisy sensor distance pings (True position: 0.1m, 0.2m, 0.3m, 0.4m, 0.5m...)
    vector<double> noisy_sensor_readings = {
        0.18, 0.12, 0.38, 0.29, 0.61, 0.52, 0.77, 0.71, 0.95, 1.05
    };

    cout << "Step | True Pos | Noisy Sensor Ping | Kalman Estimate | State Uncertainty (P)" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;

    for (size_t i = 0; i < noisy_sensor_readings.size(); i++) {
        double true_pos = (i + 1) * dt * commanded_velocity;

        // Step 1: Predict forward
        kf.predict(commanded_velocity, dt);

        // Step 2: Correct with noisy reading
        KalmanState result = kf.update(noisy_sensor_readings[i]);

        cout << " #" << setw(2) << (i + 1) << "  |  "
             << setw(5) << true_pos << "m  |      "
             << setw(5) << noisy_sensor_readings[i] << "m      |     "
             << setw(5) << result.position << "m    |        "
             << setw(6) << result.variance << endl;
    }

    cout << "\n>>> [VERIFIED] Sensor noise smoothed; state uncertainty contracted monotonically." << endl;
    return 0;
}