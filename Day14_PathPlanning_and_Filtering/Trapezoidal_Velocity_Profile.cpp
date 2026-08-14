#include <iostream>
#include <algorithm>

using namespace std;

class TrapezoidalProfile {
private:
    float max_velocity; // Maximum cruise speed (m/s)
    float acceleration; // Accel/Decel rate (m/s^2)

public:
    TrapezoidalProfile(float max_v, float accel) : max_velocity(max_v), acceleration(accel) {}

    float calculateVelocity(float current_time_sec, float total_duration_sec) {
        float t_accel = max_velocity / acceleration;
        float t_decel = total_duration_sec - t_accel;

        // Phase 1: Constant Acceleration Ramp-Up
        if (current_time_sec < t_accel) {
            return acceleration * current_time_sec;
        }
        // Phase 2: Constant Maximum Cruise Speed
        else if (current_time_sec <= t_decel) {
            return max_velocity;
        }
        // Phase 3: Constant Deceleration Ramp-Down
        else if (current_time_sec <= total_duration_sec) {
            return max_velocity - (acceleration * (current_time_sec - t_decel));
        }
        return 0.0f; // Stopped
    }
};

int main() {
    cout << "--- DAY 14: TRAPEZOIDAL MOTOR VELOCITY PROFILER ---" << endl << endl;

    // Max speed = 2.0 m/s, Acceleration = 1.0 m/s^2
    TrapezoidalProfile motorProfiler(2.0f, 1.0f);
    float total_mission_time = 6.0f; // 6 seconds run

    cout << "Time (s) | Commanded Motor Velocity (m/s)" << endl;
    cout << "----------------------------------------" << endl;

    for (float t = 0.0f; t <= total_mission_time; t += 1.0f) {
        float v = motorProfiler.calculateVelocity(t, total_mission_time);
        cout << "  T+" << t << "s   |  " << v << " m/s" << endl;
    }

    return 0;
}