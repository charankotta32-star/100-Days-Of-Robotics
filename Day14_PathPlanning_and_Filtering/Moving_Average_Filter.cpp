#include <iostream>
#include <vector>

using namespace std;

class MovingAverageFilter {
private:
    int window_size;
    vector<float> window;
    int insert_idx;
    float current_sum;
    bool is_full;

public:
    MovingAverageFilter(int size) : window_size(size), window(size, 0.0f), insert_idx(0), current_sum(0.0f), is_full(false) {}

    float filter(float new_sample) {
        // Subtract oldest value leaving the window, add incoming sample
        current_sum -= window[insert_idx];
        window[insert_idx] = new_sample;
        current_sum += new_sample;

        insert_idx = (insert_idx + 1) % window_size;
        if (insert_idx == 0) is_full = true;

        int active_samples = is_full ? window_size : insert_idx;
        return current_sum / (float)active_samples;
    }
};

int main() {
    cout << "--- DAY 14: SLIDING WINDOW MOVING AVERAGE FILTER ---" << endl << endl;

    // Window size = 3 samples
    MovingAverageFilter signalFilter(3);

    // Simulated noisy sensor stream (e.g., mmWave distance with jitter spikes)
    vector<float> raw_signal = {3.20f, 3.25f, 4.80f /* noise spike */, 3.22f, 3.18f};

    cout << "Raw Sensor Ping  ->  Filtered Output" << endl;
    for (float sample : raw_signal) {
        float smoothed = signalFilter.filter(sample);
        cout << "  " << sample << " m         ->     " << smoothed << " m" << endl;
    }

    return 0;
}