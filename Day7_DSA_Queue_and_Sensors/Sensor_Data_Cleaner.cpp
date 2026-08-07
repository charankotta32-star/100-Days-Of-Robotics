#include <iostream>
#include <vector>

using namespace std;

class SensorFilter {
private:
    float min_range = 0.2;  // 20 cm min range
    float max_range = 8.0;  // 8 meters max range

public:
    void filterRawPings(const vector<float>& raw_pings) {
        cout << "Raw Radar Stream: [ ";
        for (float ping : raw_pings) cout << ping << "m ";
        cout << "]" << endl;

        cout << "Cleaned Stream:   [ ";
        for (float ping : raw_pings) {
            if (ping >= min_range && ping <= max_range) {
                cout << ping << "m "; // Valid data
            } else {
                cout << "REJECTED "; // Noise spike filtered
            }
        }
        cout << "]" << endl << endl;
    }
};

int main() {
    cout << "--- DAY 7: MMWAVE RADAR NOISE FILTERING ---" << endl << endl;

    SensorFilter radarFilter;
    vector<float> raw_data = {3.5, 0.05, 4.2, 15.8, 2.1, -1.0, 3.8};

    radarFilter.filterRawPings(raw_data);

    return 0;
}