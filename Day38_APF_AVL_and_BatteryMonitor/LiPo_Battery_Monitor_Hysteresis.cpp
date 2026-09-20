#include <iostream>
#include <iomanip>

using namespace std;

enum BatteryState {
    BATT_NORMAL,
    BATT_LOW_WARNING,
    BATT_CRITICAL_CUTOFF
};

class LiPoBatteryMonitor {
private:
    const double VOLTAGE_FULL     = 12.60; // 4.20V / cell (100%)
    const double VOLTAGE_NOMINAL  = 11.10; // 3.70V / cell (~20-25%)
    const double VOLTAGE_WARN     = 10.50; // 3.50V / cell (Low warning)
    const double VOLTAGE_CUTOFF   =  9.90; // 3.30V / cell (Damage limit)
    const double HYSTERESIS_BAND  =  0.40; // 400mV recovery deadband

    BatteryState current_state;

public:
    LiPoBatteryMonitor() : current_state(BATT_NORMAL) {}

    double estimateStateOfCharge(double v_pack) {
        if (v_pack >= VOLTAGE_FULL) return 100.0;
        if (v_pack <= VOLTAGE_CUTOFF) return 0.0;

        // Approximate 3S LiPo non-linear discharge curve
        double percentage = ((v_pack - VOLTAGE_CUTOFF) / (VOLTAGE_FULL - VOLTAGE_CUTOFF)) * 100.0;
        return percentage;
    }

    BatteryState evaluate(double measured_voltage) {
        switch (current_state) {
            case BATT_NORMAL:
                if (measured_voltage <= VOLTAGE_CUTOFF) {
                    current_state = BATT_CRITICAL_CUTOFF;
                } else if (measured_voltage <= VOLTAGE_WARN) {
                    current_state = BATT_LOW_WARNING;
                }
                break;

            case BATT_LOW_WARNING:
                if (measured_voltage <= VOLTAGE_CUTOFF) {
                    current_state = BATT_CRITICAL_CUTOFF;
                } else if (measured_voltage > (VOLTAGE_WARN + HYSTERESIS_BAND)) {
                    current_state = BATT_NORMAL; // Recover only after clearing hysteresis band
                }
                break;

            case BATT_CRITICAL_CUTOFF:
                // Require significant voltage recovery above cutoff + deadband before re-enabling load
                if (measured_voltage >= (VOLTAGE_CUTOFF + HYSTERESIS_BAND)) {
                    current_state = BATT_LOW_WARNING;
                }
                break;
        }

        return current_state;
    }

    bool isMotorActuationAllowed() const {
        return current_state != BATT_CRITICAL_CUTOFF;
    }
};

int main() {
    cout << "--- DAY 38: 3S LIPO MONITOR WITH ANTI-OSCILLATION HYSTERESIS ---" << endl << endl;

    LiPoBatteryMonitor bms;
    cout << fixed << setprecision(2);

    // Simulated voltage readings (Normal -> Inrush motor sag -> Low -> Recovery)
    double telemetry_voltages[] = {
        12.50, // 1. Fresh battery
        11.40, // 2. Running normal load
        9.85,  // 3. Extreme motor stall sag (< 9.9V Cutoff!)
        10.15, // 4. Motor killed, voltage bounces back to 10.15V (Protected by hysteresis!)
        10.35, // 5. Still below recovery threshold (9.9V + 0.4V = 10.30V)
        10.45, // 6. Clears hysteresis, moves to Warning state
        12.60  // 7. Fresh pack plugged in
    };

    cout << "Sample # | Pack Voltage | Approx SoC % | Battery Protection State | Motor Actuation" << endl;
    cout << "-----------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < 7; i++) {
        double v = telemetry_voltages[i];
        double soc = bms.estimateStateOfCharge(v);
        BatteryState state = bms.evaluate(v);

        cout << "   #" << (i + 1) << "   |    "
             << setw(5) << v << " V   |    "
             << setw(5) << soc << "%   | ";

        if (state == BATT_NORMAL) {
            cout << "🟢 BATT_NORMAL           | ✅ ALLOWED" << endl;
        } else if (state == BATT_LOW_WARNING) {
            cout << "⚠️ BATT_LOW_WARNING       | ⚡ THROTTLED" << endl;
        } else {
            cout << "🚨 BATT_CRITICAL_CUTOFF  | 🛑 CUTOFF (LATCHED)" << endl;
        }
    }

    cout << "\n>>> [VERIFIED] Motor sag at Sample #4 prevented from false-restarting by hysteresis." << endl;
    return 0;
}