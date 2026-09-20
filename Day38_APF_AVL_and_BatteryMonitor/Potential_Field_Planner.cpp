#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

struct Vector2D {
    double x;
    double y;

    Vector2D operator+(const Vector2D& other) const { return {x + other.x, y + other.y}; }
    Vector2D operator-(const Vector2D& other) const { return {x - other.x, y - other.y}; }
    Vector2D operator*(double scalar) const { return {x * scalar, y * scalar}; }
    double magnitude() const { return sqrt(x * x + y * y); }
};

struct Obstacle {
    Vector2D pos;
    double influence_radius; // d0
};

class PotentialFieldPlanner {
private:
    double K_att;
    double K_rep;

public:
    PotentialFieldPlanner(double k_a = 1.0, double k_r = 50.0) : K_att(k_a), K_rep(k_r) {}

    // Computes net steering force vector
    Vector2D computeNetForce(const Vector2D& robot_pos, const Vector2D& goal_pos, const vector<Obstacle>& obstacles) {
        // 1. Attractive Force towards Goal
        Vector2D d_goal = goal_pos - robot_pos;
        double dist_to_goal = d_goal.magnitude();

        Vector2D f_att = {0.0, 0.0};
        if (dist_to_goal > 0.01) {
            f_att = d_goal * (K_att);
        }

        // 2. Repulsive Forces from Obstacles
        Vector2D f_rep_total = {0.0, 0.0};

        for (const auto& obs : obstacles) {
            Vector2D d_obs = robot_pos - obs.pos;
            double dist_to_obs = d_obs.magnitude();

            if (dist_to_obs < obs.influence_radius && dist_to_obs > 0.05) {
                // Potential gradient: K_rep * (1/d - 1/d0) * (1/d^2)
                double factor = K_rep * ((1.0 / dist_to_obs) - (1.0 / obs.influence_radius)) * (1.0 / (dist_to_obs * dist_to_obs));
                Vector2D unit_rep = {d_obs.x / dist_to_obs, d_obs.y / dist_to_obs};
                f_rep_total = f_rep_total + (unit_rep * factor);
            }
        }

        // Net Steering Vector
        return f_att + f_rep_total;
    }
};

int main() {
    cout << "--- DAY 38: ARTIFICIAL POTENTIAL FIELD (APF) PLANNER ---" << endl << endl;

    PotentialFieldPlanner planner(1.0, 20.0);
    cout << fixed << setprecision(2);

    Vector2D goal = {10.0, 10.0};
    vector<Obstacle> obstacles = {
        {{5.0, 5.0}, 3.0}, // Obstacle blocking the direct diagonal path
        {{3.0, 4.0}, 2.0}
    };

    // Robot positions along test trajectory
    vector<Vector2D> test_positions = {
        {0.0, 0.0}, // Far away from obstacles
        {3.5, 3.5}, // Entering obstacle field
        {4.5, 4.5}, // Very close to Obstacle 1 (high repulsion expected)
        {7.0, 7.0}  // Clear of obstacles, pulling towards goal
    };

    cout << "Goal Target: (" << goal.x << ", " << goal.y << ")\n";
    cout << "Step | Robot Pose (X, Y) | Net Force (Fx, Fy) | Force Mag | Commanded Heading" << endl;
    cout << "-------------------------------------------------------------------------------" << endl;

    for (size_t i = 0; i < test_positions.size(); i++) {
        Vector2D pos = test_positions[i];
        Vector2D f_net = planner.computeNetForce(pos, goal, obstacles);
        double heading_deg = atan2(f_net.y, f_net.x) * (180.0 / M_PI);

        cout << " #" << (i + 1) << "  | (" 
             << setw(4) << pos.x << ", " << setw(4) << pos.y << ")       | (" 
             << setw(5) << f_net.x << ", " << setw(5) << f_net.y << ")      |   " 
             << setw(6) << f_net.magnitude() << "  | " 
             << setw(6) << heading_deg << "°" << endl;
    }

    return 0;
}