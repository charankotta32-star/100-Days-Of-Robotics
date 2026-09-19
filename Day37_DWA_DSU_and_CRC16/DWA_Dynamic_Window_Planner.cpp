#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

struct DynamicWindow {
    double v_min;
    double v_max;
    double w_min;
    double w_max;
};

struct VelocityPair {
    double v;
    double w;
};

class DWAKinematicsPlanner {
private:
    double max_speed;       // m/s
    double min_speed;       // m/s (0 for forward only, negative for reverse)
    double max_yaw_rate;    // rad/s
    double max_accel;       // m/s^2
    double max_yaw_accel;   // rad/s^2

public:
    DWAKinematicsPlanner(double v_max, double v_min, double w_max, double a_v, double a_w)
        : max_speed(v_max), min_speed(v_min), max_yaw_rate(w_max),
          max_accel(a_v), max_yaw_accel(a_w) {}

    // Computes the intersection between global vehicle limits and dynamic reachability
    DynamicWindow calculateDynamicWindow(double current_v, double current_w, double dt) {
        // Dynamic window from vehicle acceleration limits
        double v_dyn_min = current_v - (max_accel * dt);
        double v_dyn_max = current_v + (max_accel * dt);
        double w_dyn_min = current_w - (max_yaw_accel * dt);
        double w_dyn_max = current_w + (max_yaw_accel * dt);

        // Intersection with absolute physical robot constraints
        DynamicWindow window;
        window.v_min = max(min_speed, v_dyn_min);
        window.v_max = min(max_speed, v_dyn_max);
        window.w_min = max(-max_yaw_rate, w_dyn_min);
        window.w_max = min(max_yaw_rate, w_dyn_max);

        return window;
    }

    // Samples candidate (v, w) pairs for local trajectory rollout
    vector<VelocityPair> sampleVelocities(const DynamicWindow& dw, int v_samples, int w_samples) {
        vector<VelocityPair> candidates;
        double v_step = (dw.v_max - dw.v_min) / max(1, v_samples - 1);
        double w_step = (dw.w_max - dw.w_min) / max(1, w_samples - 1);

        for (int i = 0; i < v_samples; i++) {
            double v = dw.v_min + (i * v_step);
            for (int j = 0; j < w_samples; j++) {
                double w = dw.w_min + (j * w_step);
                candidates.push_back({v, w});
            }
        }
        return candidates;
    }
};

int main() {
    cout << "--- DAY 37: DWA DYNAMIC VELOCITY WINDOW PLANNER ---" << endl << endl;

    // Robot params: Max v = 1.5 m/s, Max w = 2.0 rad/s, Accel = 1.0 m/s^2, Yaw Accel = 3.0 rad/s^2
    DWAKinematicsPlanner dwa(1.5, 0.0, 2.0, 1.0, 3.0);
    cout << fixed << setprecision(3);

    double current_v = 0.5; // Cruising at 0.5 m/s
    double current_w = 0.0; // Straight heading
    double dt = 0.1;        // 100ms planning loop

    DynamicWindow dw = dwa.calculateDynamicWindow(current_v, current_w, dt);

    cout << "Current State: v = " << current_v << " m/s, w = " << current_w << " rad/s | dt = " << dt << "s\n\n";
    cout << "Reachable Dynamic Window in Next Time Step:\n";
    cout << "  • Linear  Velocity Range: [" << dw.v_min << ", " << dw.v_max << "] m/s\n";
    cout << "  • Angular Velocity Range: [" << dw.w_min << ", " << dw.w_max << "] rad/s\n\n";

    // Sample 3x3 trajectory candidates
    auto candidates = dwa.sampleVelocities(dw, 3, 3);
    cout << "Sampled Trajectory Candidates for Obstacle Rollout:\n";
    cout << "   Index | Linear (m/s) | Angular (rad/s) | Steering Action\n";
    cout << "   --------------------------------------------------------\n";
    for (size_t i = 0; i < candidates.size(); i++) {
        string action = "Straight";
        if (candidates[i].w > 0.05) action = "Turn Left";
        else if (candidates[i].w < -0.05) action = "Turn Right";

        cout << "    #" << setw(2) << (i + 1) << "  |     "
             << setw(6) << candidates[i].v << "   |     "
             << setw(6) << candidates[i].w << "    | " << action << "\n";
    }

    return 0;
}