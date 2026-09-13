#include <iostream>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <thread>

using namespace std;

struct Pose2D {
    double x = 0.0;
    double y = 0.0;
    double theta = 0.0; // Radians
};

class RobustOdometry {
private:
    double wheel_radius;
    double track_width; // L: Must match kinematics exactly (0.22m)
    Pose2D current_pose;

    // Real-world clock tracking
    chrono::steady_clock::time_point last_update_time;
    bool first_update;

public:
    // Fixed: Track width is now a parameterized variable, preventing hardcoded mismatches
    RobustOdometry(double r, double l) : wheel_radius(r), track_width(l), first_update(true) {}

    void updatePose(double left_wheel_rad_s, double right_wheel_rad_s) {
        auto now = chrono::steady_clock::now();

        if (first_update) {
            last_update_time = now;
            first_update = false;
            return;
        }

        // 1. Calculate actual wall-clock dt in seconds (handles dropped packets natively!)
        chrono::duration<double> elapsed = now - last_update_time;
        double dt = elapsed.count();
        last_update_time = now;

        // OmniLink Bugfix: Flag dropped packets (Expected 20Hz = 0.05s. If > 0.08s, we dropped a frame)
        if (dt > 0.08) {
            cout << "⚠️ [JITTER WARNING] Dropped frame detected! Time since last update: " << dt * 1000.0 << " ms\n";
        }

        // 2. Kinematics using accurate track_width
        double v_left = left_wheel_rad_s * wheel_radius;
        double v_right = right_wheel_rad_s * wheel_radius;

        double v_center = (v_right + v_left) / 2.0;
        double omega = (v_right - v_left) / track_width;

        // 3. Integrate to update Pose
        current_pose.theta += omega * dt;

        // Normalize theta between -PI and PI
        while (current_pose.theta > M_PI) current_pose.theta -= 2.0 * M_PI;
        while (current_pose.theta <= -M_PI) current_pose.theta += 2.0 * M_PI;

        current_pose.x += v_center * cos(current_pose.theta) * dt;
        current_pose.y += v_center * sin(current_pose.theta) * dt;
    }

    void displayPose() const {
        cout << " -> Pose | X: " << setw(5) << current_pose.x
             << "m | Y: " << setw(5) << current_pose.y
             << "m | Heading: " << setw(6) << (current_pose.theta * 180.0 / M_PI) << " deg\n";
    }
};

int main() {
    cout << "--- DAY 34: OMNILINK ODOMETRY PATCH (WALL-CLOCK INTEGRATION) ---" << endl << endl;

    // Fixed Bug: Track width correctly set to 0.22m to match Kinematics
    RobustOdometry odom(0.05, 0.22);
    cout << fixed << setprecision(4);

    cout << "[INIT] Odometry system started at 20Hz expectation...\n";
    odom.updatePose(0.0, 0.0); // Init clock

    for (int step = 1; step <= 4; step++) {
        // Moved inside the loop to clear the Scope Warnings!
        double wl = 15.0;
        double wr = 16.0; // Slightly faster right wheel to curve left

        cout << "\nStep " << step << ": ";

        // Simulating a dropped packet on Step 3
        if (step == 3) {
            cout << "Simulating Network Drop (100ms latency)...\n";
            this_thread::sleep_for(chrono::milliseconds(100)); // Cross-platform sleep
        } else {
            this_thread::sleep_for(chrono::milliseconds(50));  // Cross-platform sleep
        }

        odom.updatePose(wl, wr);
        odom.displayPose();
    }

    return 0;
}