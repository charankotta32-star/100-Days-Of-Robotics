#include <iostream>
#include <algorithm>

using namespace std;

class BatteryMonitor {
private:
    const float V_MAX = 12.6f; // 100% Charged (3S LiPo: 4.2V/cell)
    const float V_NOM = 11.1f; // ~50% Nominal (3S LiPo: 3.7V/cell)
    const float V_MIN = 9.9f;  // 0% Critical Low (3S LiPo: 3.3V/cell cutoff)

public:
    int estimateStateOfCharge(float measured_voltage) {
        if (measured_voltage >= V_MAX) return 100;
        if (measured_voltage <= V_MIN) return 0;

        // Piecewise linear interpolation across 3S LiPo discharge curve
        float soc = 0.0f;
        if (measured_voltage >= V_NOM) {
            // Upper curve: 11.1V to 12.6V maps to 50% - 100%
            soc = 50.0f + ((measured_voltage - V_NOM) / (V_MAX - V_NOM)) * 50.0f;
        } else {
            // Lower curve: 9.9V to 11.1V maps to 0% - 50%
            soc = ((measured_voltage - V_MIN) / (V_NOM - V_MIN)) * 50.0f;
        }

        return (int)soc;
    }

    void evaluateSafety(float voltage) {
        int soc = estimateStateOfCharge(voltage);
        cout << "[BMS TELEMETRY] Pack Voltage: " << voltage << "V -> Estimated SoC: " << soc << "%";

        if (voltage < 10.2f) {
            cout << " >>> [CRITICAL WARNING] Low Battery Brownout Imminent! Rerouting to Dock." << endl;
        } else if (voltage < 11.1f) {
            cout << " >>> [STATUS] Yellow Battery: Nominal Discharge." << endl;
        } else {
            cout << " >>> [STATUS] Green Battery: Optimal Operating Voltage." << endl;
        }
    }
};

int main() {
    cout << "--- DAY 19: ROBOT 3S LIPO BATTERY STATE OF CHARGE ESTIMATOR ---" << endl << endl;

    BatteryMonitor bms;

    bms.evaluateSafety(12.5f);
    bms.evaluateSafety(11.4f);
    bms.evaluateSafety(10.8f);
    bms.evaluateSafety(10.0f); // Triggers low battery warning

    return 0;
}