#include <iostream>
#include <vector>

using namespace std;

class BandpassFilter {
private:
    float min_freq_hz = 0.75; // 45 BPM
    float max_freq_hz = 2.50; // 150 BPM

public:
    // Simple 1st-Order Bandpass Simulation
    float processSample(float raw_signal, float prev_signal) {
        float delta = raw_signal - prev_signal;

        // Suppress static DC offset and ultra-high frequency noise
        if (delta > -0.05f && delta < 0.05f) {
            return 0.0f; // Attenuate flatline noise
        }
        return raw_signal * 0.85f; // Filtered output signal
    }
};

int main() {
    cout << "--- DAY 13: BANDPASS SIGNAL FILTERING ---" << endl << endl;

    BandpassFilter filter;
    vector<float> raw_rppg_stream = {0.12, 0.14, 0.28, 0.35, 0.10, 0.02, 0.15};

    cout << "Raw Signal Stream -> Filtered Pulse Stream:" << endl;
    float prev = 0.0;
    for (float sample : raw_rppg_stream) {
        float clean = filter.processSample(sample, prev);
        cout << "  Raw: " << sample << "V  | Filtered: " << clean << "V" << endl;
        prev = sample;
    }

    return 0;
}