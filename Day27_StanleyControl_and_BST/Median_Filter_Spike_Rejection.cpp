#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>
using namespace std;

class MedianFilter3Tap {
private:
    double buffer[3];
    int count;

public:
    MedianFilter3Tap() : count(0) {
        buffer[0] = buffer[1] = buffer[2] = 0.0;
    }

    double filter(double new_sample) {
        // Shift window
        buffer[0] = buffer[1];
        buffer[1] = buffer[2];
        buffer[2] = new_sample;
        count++;

        if (count < 3) {
            return new_sample; // Pass through until window fills
        }

        // Copy and find median of 3 elements (Middle element after sort)
        double sorted_window[3] = { buffer[0], buffer[1], buffer[2] };

        // 3-Element Sorting Network (Faster than full std::sort)
        if (sorted_window[0] > sorted_window[1]) swap(sorted_window[0], sorted_window[1]);
        if (sorted_window[1] > sorted_window[2]) swap(sorted_window[1], sorted_window[2]);
        if (sorted_window[0] > sorted_window[1]) swap(sorted_window[0], sorted_window[1]);

        return sorted_window[1]; // Return Median
    }
};

int main() {
    cout << "--- DAY 27: 3-TAP SENSOR DESPIKING MEDIAN FILTER ---" << endl << endl;

    MedianFilter3Tap medFilter;
    cout << fixed << setprecision(1);

    // Simulated noisy ultrasonic sensor stream with wild random reflection spikes
    vector<double> raw_stream = {
        35.2, 35.8, 400.0 /* Multipath reflection spike */,
        36.1, 35.5, 0.0   /* False zero drop */,
        35.9, 36.4
    };

    cout << "Sample # | Raw Sensor Reading | Clean Filtered Output | Filter Status" << endl;
    cout << "---------------------------------------------------------------------" << endl;

    for (size_t i = 0; i < raw_stream.size(); i++) {
        double raw = raw_stream[i];
        double clean = medFilter.filter(raw);

        cout << "   #" << (i + 1) << "   |      "
             << setw(5) << raw << " cm     |        "
             << setw(5) << clean << " cm     | ";

        if (fabs(raw - clean) > 50.0) {
            cout << "🚨 Spike Rejected!" << endl;
        } else {
            cout << "🟢 Signal Clean" << endl;
        }
    }

    return 0;
}