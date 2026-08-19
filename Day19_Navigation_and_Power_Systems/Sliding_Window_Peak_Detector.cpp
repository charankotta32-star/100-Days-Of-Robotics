#include <iostream>
#include <vector>
#include <deque>

using namespace std;

class PeakEnvelopeDetector {
public:
    void trackPeaks(const vector<float>& signal, int window_size) {
        deque<int> dq; // Stores indices of useful peak elements

        cout << "Raw Signal Pings -> Sliding Window Peak Envelope (Window=" << window_size << "):" << endl;

        for (int i = 0; i < signal.size(); i++) {
            // Remove elements falling outside current sliding window
            while (!dq.empty() && dq.front() <= i - window_size) {
                dq.pop_front();
            }

            // Remove smaller elements (they cannot be the maximum in this window)
            while (!dq.empty() && signal[dq.back()] <= signal[i]) {
                dq.pop_back();
            }

            dq.push_back(i);

            // Display maximum once the first window is populated
            if (i >= window_size - 1) {
                cout << "Sample " << i << " [" << signal[i] << "V] -> Window Peak: "
                     << signal[dq.front()] << "V" << endl;
            }
        }
    }
};

int main() {
    cout << "--- DAY 19: DEQUE SLIDING WINDOW PEAK ENVELOPE (ARANYA DSP) ---" << endl << endl;

    PeakEnvelopeDetector detector;
    // Simulated piezo vibration response waveform
    vector<float> piezo_samples = {0.2f, 0.5f, 2.8f /* Peak tap */, 1.9f, 0.9f, 0.4f, 0.1f};

    detector.trackPeaks(piezo_samples, 3);

    return 0;
}