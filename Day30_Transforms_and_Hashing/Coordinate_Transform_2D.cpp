#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

struct Point2D {
    double x, y;
};

class CoordinateTransformer {
public:
    // Transforms a local sensor reading into the global arena map frame
    Point2D transformToGlobal(Point2D local_point, Point2D robot_pos, double robot_heading_deg) {
        // Convert heading to radians
        double theta = robot_heading_deg * (M_PI / 180.0);

        // 1. Rotation Matrix: Rotate the local point by the robot's heading
        double rotated_x = (local_point.x * cos(theta)) - (local_point.y * sin(theta));
        double rotated_y = (local_point.x * sin(theta)) + (local_point.y * cos(theta));

        // 2. Translation: Add the robot's current position in the world
        Point2D global_point;
        global_point.x = rotated_x + robot_pos.x;
        global_point.y = rotated_y + robot_pos.y;

        return global_point;
    }
};

int main() {
    cout << "--- DAY 30: 2D ROTATION & TRANSLATION MATRICES (MATH) ---" << endl << endl;

    CoordinateTransformer tf;
    cout << fixed << setprecision(2);

    Point2D robot_global_pos = {5.0, 5.0}; // Robot is at (5,5) in the arena
    double robot_heading = 90.0;           // Robot is facing strictly North (90 degrees)

    // LiDAR detects an obstacle exactly 2 meters straight ahead (Local X=2, Y=0)
    Point2D lidar_ping_local = {2.0, 0.0};

    Point2D obstacle_global = tf.transformToGlobal(lidar_ping_local, robot_global_pos, robot_heading);

    cout << "Robot State:      Pos(X: " << robot_global_pos.x << ", Y: " << robot_global_pos.y
         << ") | Heading: " << robot_heading << "°" << endl;
    cout << "Local Sensor Hit: (X: " << lidar_ping_local.x << ", Y: " << lidar_ping_local.y << ") relative to robot" << endl;
    cout << ">>> Global Map Coordinate: (X: " << obstacle_global.x << ", Y: " << obstacle_global.y << ") <<<" << endl;

    return 0;
}