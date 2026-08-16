#include <iostream>
#include <cmath>

using namespace std;

struct Waypoint {
    float x;
    float y;
};

class PurePursuitTracker {
private:
    float wheelbase_meters = 0.25; // 25cm robot length

public:
    // Computes steering angle (in degrees) to steer toward lookahead waypoint
    float computeSteeringAngle(Waypoint target, float lookahead_dist) {
        // Alpha: Angle between robot heading (X-axis) and the waypoint vector
        float alpha = atan2(target.y, target.x);

        // Pure Pursuit Curvature equation: curvature = (2 * sin(alpha)) / Ld
        float curvature = (2.0f * sin(alpha)) / lookahead_dist;

        // Steering Angle delta = atan(curvature * wheelbase)
        float steering_angle_rad = atan(curvature * wheelbase_meters);
        float steering_angle_deg = steering_angle_rad * (180.0f / M_PI);

        return steering_angle_deg;
    }
};

int main() {
    cout << "--- DAY 16: PURE PURSUIT AUTONOMOUS PATH TRACKER ---" << endl << endl;

    PurePursuitTracker roverSteering;

    // Target waypoint 1.5m ahead, drifted 0.6m to the left
    Waypoint target_point = {1.5f, 0.6f};
    float lookahead_distance = sqrt(target_point.x * target_point.x + target_point.y * target_point.y);

    float steer_cmd = roverSteering.computeSteeringAngle(target_point, lookahead_distance);

    cout << "Target Waypoint Relative: (X: " << target_point.x << "m, Y: " << target_point.y << "m)" << endl;
    cout << "Lookahead Distance: " << lookahead_distance << "m" << endl;
    cout << ">>> Commanded Steering Adjustment: " << steer_cmd << " degrees" << endl << endl;

    return 0;
}