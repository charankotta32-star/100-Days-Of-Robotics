#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

struct MotionState {
    double time_s;
    double position_m;
    double velocity_m_s;
    double acceleration_m_s2;
    string phase;
};

class TrapezoidalProfile {
private:
    double max_v;
    double max_a;

    double t_accel;
    double t_cruise;
    double t_total;
    double target_dist;
    bool triangle_profile;

public:
    TrapezoidalProfile(double v_max, double a_max) : max_v(v_max), max_a(a_max) {}

    void planTrajectory(double distance_m) {
        target_dist = distance_m;

        // Distance needed to reach max velocity: d_accel = v^2 / (2 * a)
        double d_accel = (max_v * max_v) / (2.0 * max_a);

        // If distance is too short to reach max_v, profile becomes triangular
        if (2.0 * d_accel > distance_m) {
            triangle_profile = true;
            t_accel = sqrt(distance_m / max_a);
            t_cruise = 0.0;
            t_total = 2.0 * t_accel;
            max_v = max_a * t_accel; // Peak velocity reached
        } else {
            triangle_profile = false;
            t_accel = max_v / max_a;
            double d_cruise = distance_m - (2.0 * d_accel);
            t_cruise = d_cruise / max_v;
            t_total = (2.0 * t_accel) + t_cruise;
        }
    }

    MotionState evaluate(double t) {
        MotionState state;
        state.time_s = t;

        if (t <= 0.0) {
            return {0.0, 0.0, 0.0, 0.0, "STANDSTILL"};
        }

        // Phase 1: Acceleration
        if (t <= t_accel) {
            state.acceleration_m_s2 = max_a;
            state.velocity_m_s = max_a * t;
            state.position_m = 0.5 * max_a * t * t;
            state.phase = "ACCEL";
        }
        // Phase 2: Cruising
        else if (!triangle_profile && t <= (t_accel + t_cruise)) {
            double dt_cruise = t - t_accel;
            double d_accel = 0.5 * max_a * t_accel * t_accel;

            state.acceleration_m_s2 = 0.0;
            state.velocity_m_s = max_v;
            state.position_m = d_accel + (max_v * dt_cruise);
            state.phase = "CRUISE";
        }
        // Phase 3: Deceleration
        else if (t <= t_total) {
            double dt_decel = t - (t_accel + t_cruise);
            double d_cruise_end = target_dist - (0.5 * max_a * t_accel * t_accel);

            state.acceleration_m_s2 = -max_a;
            state.velocity_m_s = max_v - (max_a * dt_decel);
            state.position_m = d_cruise_end + (max_v * dt_decel) - (0.5 * max_a * dt_decel * dt_decel);
            state.phase = "DECEL";
        }
        // Completed
        else {
            state.acceleration_m_s2 = 0.0;
            state.velocity_m_s = 0.0;
            state.position_m = target_dist;
            state.phase = "TARGET_REACHED";
        }

        return state;
    }

    double getTotalDuration() const { return t_total; }
};

int main() {
    cout << "--- DAY 36: TRAPEZOIDAL MOTION PROFILE (ROBOTICS KINEMATICS) ---" << endl << endl;

    // Plan motion for Aegis-Rover: Target Distance = 2.0m, Max Velocity = 1.0 m/s, Max Accel = 0.5 m/s^2
    TrapezoidalProfile profile(1.0, 0.5);
    profile.planTrajectory(2.0);

    cout << fixed << setprecision(2);
    cout << "Trajectory Planned: Distance = 2.0m | Duration = " << profile.getTotalDuration() << "s\n\n";
    cout << "Time(s) | Position(m) | Velocity(m/s) | Accel(m/s^2) | Motion Phase" << endl;
    cout << "-----------------------------------------------------------------" << endl;

    for (double t = 0.0; t <= profile.getTotalDuration() + 0.2; t += 0.4) {
        MotionState s = profile.evaluate(t);
        cout << " " << setw(5) << s.time_s << "s | "
             << setw(9) << s.position_m << "m | "
             << setw(11) << s.velocity_m_s << " | "
             << setw(10) << s.acceleration_m_s2 << " | "
             << s.phase << endl;
    }

    return 0;
}