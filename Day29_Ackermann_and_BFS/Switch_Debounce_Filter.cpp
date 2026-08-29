#include <iostream>
#include <vector>

using namespace std;

class DebounceFilter {
private:
    int debounce_delay_ms;
    int time_stable_ms;
    bool last_raw_state;
    bool validated_state;

public:
    DebounceFilter(int delay_ms) : debounce_delay_ms(delay_ms), time_stable_ms(0),
                                   last_raw_state(false), validated_state(false) {}

    // Simulating a fast 1ms control loop
    bool update(bool raw_reading) {
        if (raw_reading == last_raw_state) {
            // Signal is stable, increment counter
            time_stable_ms++;
        } else {
            // Signal bounced! Reset the stability counter
            time_stable_ms = 0;
        }

        // If signal has been stable for the required delay, update the validated state
        if (time_stable_ms >= debounce_delay_ms) {
            if (raw_reading != validated_state) {
                validated_state = raw_reading;
                cout << "  ✅ [DEBOUNCE CLEARED] Valid State Change Detected: "
                     << (validated_state ? "PRESSED" : "RELEASED") << endl;
            }
        }

        last_raw_state = raw_reading;
        return validated_state;
    }
};

int main() {
    cout << "--- DAY 29: EMBEDDED SWITCH DEBOUNCE FILTER ---" << endl << endl;

    DebounceFilter bumperSensor(5); // Requires 5ms of stability to confirm state

    // Simulated 1ms readings: Bounces 0->1->0->1 before settling at 1
    vector<bool> physical_switch_bounces = {
        0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1
    };

    cout << "Time(ms) | Raw Hardware Signal | Verified Software State" << endl;
    cout << "--------------------------------------------------------" << endl;

    for (size_t t = 0; t < physical_switch_bounces.size(); t++) {
        bool raw = physical_switch_bounces[t];
        bool clean = bumperSensor.update(raw);

        cout << "  " << t << " ms   |         "
             << raw << "           |          " << clean << endl;
    }

    return 0;
}