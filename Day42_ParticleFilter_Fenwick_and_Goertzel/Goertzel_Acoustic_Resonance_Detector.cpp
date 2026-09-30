#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

class GoertzelDetector {
private:
    double sampling_rate_hz;

public:
    explicit GoertzelDetector(double sample_rate) : sampling_rate_hz(sample_rate) {}

    // Computes relative energy magnitude of a single target frequency in O(N) time with O(1) memory
    [[nodiscard]] double computeMagnitude(const vector<double>& samples, double target_freq_hz) const {
        int n_samples = samples.size();
        if (n_samples == 0) return 0.0;

        // Calculate normalized angular frequency
        double k = round((n_samples * target_freq_hz) / sampling_rate_hz);
        double omega = (2.0 * M_PI * k) / n_samples;
        double coeff = 2.0 * cos(omega);

        double s_prev1 = 0.0;
        double s_prev2 = 0.0;

        // Second-order recursive filter
        for (double sample : samples) {
            double s = sample + (coeff * s_prev1) - s_prev2;
            s_prev2 = s_prev1;
            s_prev1 = s;
        }

        // Power spectrum estimation
        double power = (s_prev1 * s_prev1) + (s_prev2 * s_prev2) - (coeff * s_prev1 * s_prev2);
        return sqrt(max(0.0, power)) / (n_samples / 2.0);
    }
};

int main() {
    cout << "--- DAY 42: GOERTZEL ACOUSTIC RESONANCE DETECTOR (PROJECT ARANYA) ---" << endl << endl;

    double sample_rate = 8000.0; // 8 kHz ADC audio sampling rate
    GoertzelDetector detector(sample_rate);
    cout << fixed << setprecision(2);

    int num_samples = 256;
    vector<double> healthy_tree_signal(num_samples);
    vector<double> decayed_tree_signal(num_samples);

    // Simulate Acoustic Signals:
    // Healthy wood generates high-frequency resonance at 850 Hz
    // Decayed wood generates low-frequency resonance at 320 Hz
    for (int i = 0; i < num_samples; i++) {
        double t = (double)i / sample_rate;
        healthy_tree_signal[i] = 1.5 * sin(2.0 * M_PI * 850.0 * t) + 0.2 * sin(2.0 * M_PI * 1500.0 * t);
        decayed_tree_signal[i] = 1.8 * sin(2.0 * M_PI * 320.0 * t) + 0.3 * sin(2.0 * M_PI * 900.0 * t);
    }

    double TARGET_HEALTHY_FREQ = 850.0; // Hz
    double TARGET_DECAY_FREQ   = 320.0; // Hz

    cout << "Analyzing Timber Sample A (Healthy Wood):\n";
    double mag_healthy_a = detector.computeMagnitude(healthy_tree_signal, TARGET_HEALTHY_FREQ);
    double mag_decay_a   = detector.computeMagnitude(healthy_tree_signal, TARGET_DECAY_FREQ);
    cout << " • 850 Hz Energy: " << setw(5) << mag_healthy_a << " | 320 Hz Energy: " << setw(5) << mag_decay_a << "\n";
    cout << " ➔ Verdict: " << (mag_healthy_a > mag_decay_a ? "🟢 STRUCTURAL INTEGRITY SOUND" : "🚨 DECAY") << "\n\n";

    cout << "Analyzing Timber Sample B (Decayed Trunk Interior):\n";
    double mag_healthy_b = detector.computeMagnitude(decayed_tree_signal, TARGET_HEALTHY_FREQ);
    double mag_decay_b   = detector.computeMagnitude(decayed_tree_signal, TARGET_DECAY_FREQ);
    cout << " • 850 Hz Energy: " << setw(5) << mag_healthy_b << " | 320 Hz Energy: " << setw(5) << mag_decay_b << "\n";
    cout << " ➔ Verdict: " << (mag_decay_b > mag_healthy_b ? "🚨 INTERNAL CAVITY DETECTED (HIGH RISK)" : "🟢 SOUND") << "\n";

    return 0;
}