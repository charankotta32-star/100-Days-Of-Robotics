#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

struct ObstaclePoint {
    double x; // meters relative to robot
    double y; // meters relative to robot
};

class VectorFieldHistogram {
private:
    int num_sectors;
    double sector_size_deg;
    double active_radius_m;
    double obstacle_threshold;
    vector<double> polar_density;

    static double normalizeAngleDeg(double deg) {
        while (deg > 180.0) deg -= 360.0;
        while (deg <= -180.0) deg += 360.0;
        return deg;
    }

public:
    VectorFieldHistogram(int sectors = 36, double radius = 4.0, double threshold = 5.0)
        : num_sectors(sectors), active_radius_m(radius), obstacle_threshold(threshold) {
        sector_size_deg = 360.0 / num_sectors;
        polar_density.assign(num_sectors, 0.0);
    }

    void buildPolarHistogram(const vector<ObstaclePoint>& point_cloud) {
        fill(polar_density.begin(), polar_density.end(), 0.0);

        for (const auto& pt : point_cloud) {
            double dist = hypot(pt.x, pt.y);
            if (dist > 0.05 && dist <= active_radius_m) {
                // Angle relative to robot front (0 degrees = Straight ahead)
                double angle_deg = atan2(pt.y, pt.x) * (180.0 / M_PI);
                if (angle_deg < 0) angle_deg += 360.0;

                int sector_idx = (int)(angle_deg / sector_size_deg) % num_sectors;

                // Weight inversely proportional to distance (closer obstacle = higher threat density)
                double density_weight = (active_radius_m - dist) * (active_radius_m - dist);
                polar_density[sector_idx] += density_weight;
            }
        }
    }

    // Selects candidate steering direction closest to goal that clears obstacle threshold
    [[nodiscard]] double selectSteeringDirection(double goal_heading_deg) const {
        double best_steer = 0.0;
        double min_diff = 1e9;
        bool valley_found = false;

        for (int i = 0; i < num_sectors; ++i) {
            if (polar_density[i] < obstacle_threshold) { // Free sector (Valley)
                double sector_center_deg = (i * sector_size_deg) + (sector_size_deg / 2.0);
                if (sector_center_deg > 180.0) sector_center_deg -= 360.0;

                double diff = abs(normalizeAngleDeg(sector_center_deg - goal_heading_deg));
                if (diff < min_diff) {
                    min_diff = diff;
                    best_steer = sector_center_deg;
                    valley_found = true;
                }
            }
        }

        if (!valley_found) {
            cout << "🚨 [VFH WARNING] Trapped! All sectors exceed safe obstacle threshold.\n";
            return 0.0;
        }

        return best_steer;
    }

    void displayHistogram() const {
        cout << "--- 1D POLAR OBSTACLE DENSITY (VFH SECTORS) ---\n";
        cout << "Sector | Angular Range | Density Score | Status\n";
        cout << "------------------------------------------------\n";
        for (int i = 0; i < num_sectors; i += 3) { // Sample every 30 degrees for display
            double start_angle = i * sector_size_deg;
            if (start_angle > 180.0) start_angle -= 360.0;

            cout << "  #" << setw(2) << i << "   | " 
                 << setw(5) << start_angle << "°        |     " 
                 << setw(5) << polar_density[i] << "     | "
                 << (polar_density[i] < obstacle_threshold ? "🟢 Free Valley" : "🛑 Blocked") << "\n";
        }
    }
};

int main() {
    cout << "--- DAY 44: VECTOR FIELD HISTOGRAM (VFH) OBSTACLE AVOIDANCE ---" << endl << endl;

    // 36 sectors (10° resolution), 4m radar horizon, threshold = 6.0
    VectorFieldHistogram vfh(36, 4.0, 6.0);
    cout << fixed << setprecision(2);

    // Goal is directly ahead at 0°
    double goal_heading = 0.0;

    // Simulated 2D LiDAR / Range sensor returns: Dense obstacle cluster right in front (x=1.5m, y=0.0m)
    vector<ObstaclePoint> sensor_points = {
        {1.5, 0.0}, {1.6, 0.2}, {1.4, -0.2}, {1.8, 0.1}, // Direct front wall
        {2.5, 0.5}, {2.8, -0.4}
    };

    vfh.buildPolarHistogram(sensor_points);
    vfh.displayHistogram();

    double commanded_steer = vfh.selectSteeringDirection(goal_heading);

    cout << "\nGoal Heading: " << goal_heading << "° (Straight Ahead)\n";
    cout << ">>> [VFH DECISION] Front blocked! Optimal valley steering direction: " 
         << commanded_steer << "° <<<\n";

    return 0;
}