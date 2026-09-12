#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Pose2D {
    double x;     // meters
    double y;     // meters
    double theta; // radians
};

struct VelocityCommand {
    double linear_v;  // m/s
    double angular_w; // rad/s
    bool reached_goal;
};

class PolarWaypointNavigator {
private:
    double K_rho;   // Distance proportional gain
    double K_alpha; // Heading proportional gain
    double goal_tolerance_m;
    double max_linear_v;
    double max_angular_w;

    // Normalizes angle to (-PI, PI] to prevent 360-degree over-spinning
    double normalizeAngle(double angle_rad) {
        while (angle_rad > M_PI)  angle_rad -= 2.0 * M_PI;
        while (angle_rad <= -M_PI) angle_rad += 2.0 * M_PI;
        return angle_rad;
    }

public:
    PolarWaypointNavigator(double k_r = 0.8, double k_a = 2.0, double tol = 0.05)
        : K_rho(k_r), K_alpha(k_a), goal_tolerance_m(tol),
          max_linear_v(1.2), max_angular_w(3.0) {}

    VelocityCommand step(const Pose2D& current_pose, double goal_x, double goal_y) {
        double dx = goal_x - current_pose.x;
        double dy = goal_y - current_pose.y;

        // 1. Distance error (rho)
        double rho = sqrt(dx * dx + dy * dy);

        // Check if destination is reached within tolerance
        if (rho <= goal_tolerance_m) {
            return {0.0, 0.0, true};
        }

        // 2. Heading error (alpha)
        double target_heading = atan2(dy, dx);
        double alpha = normalizeAngle(target_heading - current_pose.theta);

        // 3. Proportional Control Law
        double v = K_rho * rho;
        double w = K_alpha * alpha;

        // If the rover is pointing completely backward (> 90 deg), prioritize turning first
        if (fabs(alpha) > (M_PI / 2.0)) {
            v = 0.1; // Crawl while swinging nose toward waypoint
        }

        // Clamp outputs to physical motor limits
        v = max(0.0, min(max_linear_v, v));
        w = max(-max_angular_w, min(max_angular_w, w));

        return {v, w, false};
    }
};

int main() {
    cout << "--- DAY 33: POLAR COORDINATES WAYPOINT STEERING (21MMH024F) ---" << endl << endl;

    PolarWaypointNavigator nav(0.9, 2.2, 0.05);
    cout << fixed << setprecision(2);

    // Goal at (3.0m, 2.0m)
    double goal_x = 3.0;
    double goal_y = 2.0;

    // Simulated path steps from origin (0, 0) facing 0 rad (East)
    Pose2D simulated_trajectory[] = {
        {0.0, 0.0, 0.0},          // Start at origin, facing East
        {0.8, 0.5, 0.45},         // Advancing & aligning
        {1.8, 1.2, 0.58},         // Halfway
        {2.7, 1.8, 0.60},         // Approaching
        {2.98, 1.99, 0.59}        // Inside tolerance (0.05m)
    };

    cout << "Target Waypoint: (" << goal_x << "m, " << goal_y << "m)" << endl;
    cout << "Step | Current Pose (X, Y, Heading) | Commanded V (m/s) | Commanded W (rad/s) | Status" << endl;
    cout << "--------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < 5; i++) {
        Pose2D p = simulated_trajectory[i];
        VelocityCommand cmd = nav.step(p, goal_x, goal_y);

        cout << " #" << (i + 1) << "  | ("
             << setw(4) << p.x << "m, " << setw(4) << p.y << "m, "
             << setw(5) << (p.theta * 180.0 / M_PI) << "°)   |       "
             << setw(4) << cmd.linear_v << "        |        "
             << setw(5) << cmd.angular_w << "        | ";

        if (cmd.reached_goal) {
            cout << "🎯 [GOAL REACHED]" << endl;
        } else {
            cout << "🚗 Tracking" << endl;
        }
    }

    return 0;
}