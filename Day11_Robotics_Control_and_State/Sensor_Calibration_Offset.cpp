#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

class IMUCalibrator {
private:
    float x_offset = 0.0;

public:
    void calibrateZeroBias(const vector<float>& raw_samples) {
        float sum = accumulate(raw_samples.begin(), raw_samples.end(), 0.0f);
        x_offset = sum / raw_samples.size(); // Mean error offset
        cout << "[CALIBRATION] Calculated X-Axis Zero-Bias Offset: " << x_offset << "g" << endl;
    }

    float getCorrectedReading(float raw_reading) {
        return raw_reading - x_offset;
    }
};

int main() {
    cout << "--- DAY 11: MPU6050 ZERO-BIAS CALIBRATION ---" << endl << endl;

    IMUCalibrator imu;
    vector<float> calibration_pings = {0.12, 0.15, 0.14, 0.13, 0.16}; // Resting bias

    imu.calibrateZeroBias(calibration_pings);

    float raw_motion_ping = 1.45; // Actual movement
    cout << "\nRaw Motion Ping:       " << raw_motion_ping << "g" << endl;
    cout << "Corrected Motion Ping: " << imu.getCorrectedReading(raw_motion_ping) << "g" << endl;

    return 0;
}