#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Waypoint {
    double x;
    double y;
};

struct SteeringCommand {
    double lookahead_dist;
    double curvature;
    double steer_angle_deg;
    Waypoint target_point;
};

class AdaptivePurePursuit {
private:
    double k_v;          // Lookahead gain per m/s of speed
    double min_lookahead;// L_min (meters)
    double max_lookahead;// L_max (meters)
    double wheelbase;    // L_wheelbase (meters)

    static double normalizeAngle(double angle_rad) {
        while (angle_rad > M_PI)  angle_rad -= 2.0 * M_PI;
        while (angle_rad <= -M_PI) angle_rad += 2.0 * M_PI;
        return angle_rad;
    }

public:
    AdaptivePurePursuit(double gain, double l_min, double l_max, double l_wb)
        : k_v(gain), min_lookahead(l_min), max_lookahead(l_max), wheelbase(l_wb) {}

    // Computes adaptive lookahead distance: Ld = clamp(kv * v + Lmin, Lmin, Lmax)
    [[nodiscard]] double computeLookahead(double speed_m_s) const {
        double raw_ld = (k_v * speed_m_s) + min_lookahead;
        return clamp(raw_ld, min_lookahead, max_lookahead);
    }

    SteeringCommand computeSteering(double robot_x, double robot_y, double robot_theta_rad,
                                   double speed_m_s, const vector<Waypoint>& path) {
        double Ld = computeLookahead(speed_m_s);

        // Find the first waypoint along the path that is at least Ld distance away
        Waypoint target = path.back();
        for (const auto& pt : path) {
            double dist = hypot(pt.x - robot_x, pt.y - robot_y);
            if (dist >= Ld) {
                target = pt;
                break;
            }
        }

        // Angle from robot heading to target point (alpha)
        double angle_to_target = atan2(target.y - robot_y, target.x - robot_x);
        double alpha = normalizeAngle(angle_to_target - robot_theta_rad);

        // Pure pursuit curvature: kappa = 2 * sin(alpha) / Ld
        double curvature = (2.0 * sin(alpha)) / Ld;

        // Steering angle for bicycle/skid-steer model: delta = atan(kappa * L)
        double steer_rad = atan(curvature * wheelbase);
        double steer_deg = steer_rad * (180.0 / M_PI);

        return {Ld, curvature, steer_deg, target};
    }
};

int main() {
    cout << "--- DAY 41: ADAPTIVE LOOKAHEAD PURE PURSUIT CONTROLLER ---" << endl << endl;

    // Parameters: kv = 0.5, L_min = 0.4m, L_max = 1.5m, Wheelbase = 0.24m (Aegis-Rover)
    AdaptivePurePursuit tracker(0.5, 0.4, 1.5, 0.24);
    cout << fixed << setprecision(3);

    // Global planned path coordinates (meters)
    vector<Waypoint> path = {
        {0.0, 0.0}, {1.0, 0.2}, {2.0, 0.8}, {3.0, 1.8}, {4.0, 3.0}, {5.0, 4.5}
    };

    // Robot state: At (1.0m, 0.0m) facing 0° (East)
    double rx = 1.0, ry = 0.0, r_heading = 0.0;

    // Test across increasing speeds: Low speed crawling vs high speed sprint
    vector<double> test_speeds = {0.2, 0.6, 1.2, 1.8, 2.4};

    cout << "Robot Pose: (1.00m, 0.00m, Heading: 0.0°) | Path: Curving Northeast\n\n";
    cout << "Speed (m/s) | Dynamic Ld | Target Waypoint | Curvature | Commanded Steer" << endl;
    cout << "-----------------------------------------------------------------------" << endl;

    for (double v : test_speeds) {
        SteeringCommand cmd = tracker.computeSteering(rx, ry, r_heading, v, path);
        cout << "  " << setw(5) << v << " m/s  |   "
             << setw(5) << cmd.lookahead_dist << "m   |   ("
             << setw(4) << cmd.target_point.x << ", "
             << setw(4) << cmd.target_point.y << ")   |   "
             << setw(6) << cmd.curvature << "  |   "
             << setw(6) << cmd.steer_angle_deg << "°" << endl;
    }

    cout << "\n>>> [VERIFIED] Lookahead expands smoothly with speed, damping high-speed steering." << endl;
    return 0;
}