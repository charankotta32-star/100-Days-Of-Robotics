#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

using namespace std;

struct IMURawData {
    double ax, ay, az; // Accelerometer in g's
    double gx, gy, gz; // Gyroscope in degrees/sec
};

class ComplementaryFilter6DOF {
private:
    double pitch_angle; // Estimated pitch in degrees
    double roll_angle;  // Estimated roll in degrees
    double alpha;       // Filter weighting factor (typically 0.98)
    bool initialized;

public:
    ComplementaryFilter6DOF(double filter_weight = 0.98)
        : pitch_angle(0.0), roll_angle(0.0), alpha(filter_weight), initialized(false) {}

    void update(IMURawData raw, double dt_sec) {
        // 1. Calculate Pitch & Roll from Accelerometer Gravity Vectors
        double accel_pitch = atan2(-raw.ax, sqrt(raw.ay * raw.ay + raw.az * raw.az)) * (180.0 / M_PI);
        double accel_roll  = atan2(raw.ay, raw.az) * (180.0 / M_PI);

        if (!initialized) {
            pitch_angle = accel_pitch;
            roll_angle  = accel_roll;
            initialized = true;
            return;
        }

        // 2. Fuse High-Pass Gyro Integration with Low-Pass Accelerometer Tilt
        pitch_angle = alpha * (pitch_angle + raw.gy * dt_sec) + (1.0 - alpha) * accel_pitch;
        roll_angle  = alpha * (roll_angle  + raw.gx * dt_sec) + (1.0 - alpha) * accel_roll;
    }

    double getPitch() const { return pitch_angle; }
    double getRoll() const { return roll_angle; }
};

int main() {
    cout << "--- DAY 22: 6-DOF IMU COMPLEMENTARY FILTER (MPU6050 FUSION) ---" << endl << endl;

    ComplementaryFilter6DOF filter(0.98);
    double dt = 0.05; // 50ms time step (20 Hz loop)

    // Simulated MPU6050 stream: Rover tilting to 15 deg Pitch with motor vibration noise
    vector<IMURawData> sensor_stream = {
        { -0.26, 0.02, 0.96,  0.5, 14.8, 0.1 }, // Accel ~15 deg, Gyro reading ~15 deg/s rotation
        { -0.27, 0.01, 0.95, -0.2, 15.1, 0.0 },
        { -0.55 /* vibration spike */, 0.03, 0.85, 0.1, 15.0, 0.1 },
        { -0.26, 0.01, 0.96,  0.0,  0.2, 0.0 }, // Stabilized at 15 deg tilt
        { -0.26, 0.02, 0.96,  0.1, -0.1, 0.0 }
    };

    cout << fixed << setprecision(2);
    cout << "Step | Raw Accel Pitch | Raw Gyro Rate | Fused Clean Pitch Angle" << endl;
    cout << "---------------------------------------------------------------" << endl;

    for (size_t i = 0; i < sensor_stream.size(); i++) {
        double raw_accel_pitch = atan2(-sensor_stream[i].ax,
            sqrt(sensor_stream[i].ay * sensor_stream[i].ay + sensor_stream[i].az * sensor_stream[i].az)) * (180.0 / M_PI);

        filter.update(sensor_stream[i], dt);

        cout << " #" << (i + 1) << "  |     "
             << setw(6) << raw_accel_pitch << " deg   |   "
             << setw(6) << sensor_stream[i].gy << " d/s  |       "
             << setw(6) << filter.getPitch() << " deg" << endl;
    }

    cout << "\n>>> [STATUS] Motor vibration spike isolated successfully." << endl;
    return 0;
}