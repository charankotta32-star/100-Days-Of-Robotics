#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

class SharpIRSensor {
private:
    const double V_REF = 3.3;        // ADC Reference Voltage (3.3V on ESP32)
    const double ADC_RESOLUTION = 4095.0; // 12-bit ADC max value
    const double K_FACTOR = 27.86;   // Empirical sensor calibration constant
    const double V_OFFSET = 0.10;    // Voltage zero-bias offset

public:
    // Converts raw 12-bit ADC integer (0-4095) into calibrated distance (cm)
    double readDistanceCm(int raw_adc) {
        // 1. Convert ADC integer to real voltage
        double voltage = (raw_adc / ADC_RESOLUTION) * V_REF;

        // 2. Blind-zone check (Voltage spikes > 2.8V indicate object too close < 10cm)
        if (voltage >= 2.8) {
            cout << "⚠️ [BLIND ZONE ALERT] Object closer than 10cm! ";
            return 9.9; // Clamped to minimum sensing threshold
        }

        // 3. Far-horizon check (Voltage < 0.4V means no obstacle within 80cm)
        if (voltage <= 0.40) {
            return 80.0; // Max sensing horizon
        }

        // 4. Non-Linear Inverse Power Curve: Distance = K / (V - V_offset)
        double distance_cm = K_FACTOR / (voltage - V_OFFSET);
        return distance_cm;
    }
};

int main() {
    cout << "--- DAY 25: ANALOG IR SENSOR CALIBRATION & VOLTAGE MAPPING ---" << endl << endl;

    SharpIRSensor irSensor;
    cout << fixed << setprecision(1);

    // Simulated 12-bit ADC readings from ESP32 analogRead(A0)
    int adc_readings[] = { 3600 /* Blind zone */, 2400, 1650, 950, 420 /* Far */ };

    cout << "Raw 12-bit ADC | Sensor Voltage | Calibrated Distance (cm)" << endl;
    cout << "---------------------------------------------------------" << endl;

    for (int i = 0; i < 5; i++) {
        int raw = adc_readings[i];
        double voltage = (raw / 4095.0) * 3.3;
        double dist = irSensor.readDistanceCm(raw);

        cout << "    " << setw(4) << raw << " pt    |     "
             << setw(4) << voltage << " V    |        "
             << setw(4) << dist << " cm" << endl;
    }

    return 0;
}