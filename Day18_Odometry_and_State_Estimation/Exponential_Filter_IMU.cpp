#include <iostream>
#include <vector>

using namespace std;

class ExponentialFilter {
private:
    float alpha;
    float filtered_value;
    bool initialized;

public:
    ExponentialFilter(float weight) : alpha(weight), filtered_value(0.0f), initialized(false) {}

    float update(float raw_measurement) {
        if (!initialized) {
            filtered_value = raw_measurement;
            initialized = true;
            return filtered_value;
        }
        // EMA formula: y[k] = alpha * x[k] + (1 - alpha) * y[k-1]
        filtered_value = (alpha * raw_measurement) + ((1.0f - alpha) * filtered_value);
        return filtered_value;
    }
};

int main() {
    cout << "--- DAY 18: EXPONENTIAL LOW-PASS SENSOR FILTER ---" << endl << endl;

    // Alpha = 0.3 (filters 70% of high-frequency noise spikes)
    ExponentialFilter imuFilter(0.3f);

    vector<float> raw_pings = {1.02f, 1.05f, 2.80f /* noise spike */, 1.08f, 1.03f};

    cout << "Raw IMU Reading  ->  Filtered Output" << endl;
    for (float ping : raw_pings) {
        float smoothed = imuFilter.update(ping);
        cout << "  " << ping << " g        ->     " << smoothed << " g" << endl;
    }

    return 0;
}