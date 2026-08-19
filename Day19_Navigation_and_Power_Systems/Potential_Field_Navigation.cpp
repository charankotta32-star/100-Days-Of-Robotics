#include <iostream>
#include <cmath>

using namespace std;

struct Vector2D {
    float x, y;
};

class PotentialFieldPlanner {
private:
    float k_att = 1.0f;  // Attractive gain
    float k_rep = 5.0f;  // Repulsive gain
    float d_influence = 2.0f; // Obstacle influence threshold (2 meters)

public:
    Vector2D computeTotalForce(Vector2D robot, Vector2D goal, Vector2D obstacle) {
        // 1. Attractive Force toward Goal: F_att = k_att * (goal - robot)
        Vector2D f_att = {
            k_att * (goal.x - robot.x),
            k_att * (goal.y - robot.y)
        };

        // 2. Repulsive Force away from Obstacle
        float dist_to_obs = sqrt(pow(robot.x - obstacle.x, 2) + pow(robot.y - obstacle.y, 2));
        Vector2D f_rep = {0.0f, 0.0f};

        if (dist_to_obs < d_influence && dist_to_obs > 0.01f) {
            float rep_factor = k_rep * (1.0f / dist_to_obs - 1.0f / d_influence) * (1.0f / pow(dist_to_obs, 2));
            f_rep.x = rep_factor * (robot.x - obstacle.x);
            f_rep.y = rep_factor * (robot.y - obstacle.y);
        }

        // 3. Resultant Force Vector
        return {f_att.x + f_rep.x, f_att.y + f_rep.y};
    }
};

int main() {
    cout << "--- DAY 19: ARTIFICIAL POTENTIAL FIELD NAVIGATION ---" << endl << endl;

    PotentialFieldPlanner apf;

    Vector2D robot = {0.0f, 0.0f};
    Vector2D goal  = {5.0f, 5.0f};
    Vector2D obstacle = {1.0f, 1.0f}; // Obstacle blocking direct path

    Vector2D net_force = apf.computeTotalForce(robot, goal, obstacle);
    float target_heading_deg = atan2(net_force.y, net_force.x) * (180.0f / M_PI);

    cout << "Robot Position: (" << robot.x << ", " << robot.y << ")" << endl;
    cout << "Goal Target:    (" << goal.x << ", " << goal.y << ")" << endl;
    cout << "Obstacle Alert: (" << obstacle.x << ", " << obstacle.y << ") within repulsion zone!" << endl << endl;
    cout << "Resultant Force Vector: [" << net_force.x << " N, " << net_force.y << " N]" << endl;
    cout << ">>> Steering Vector Angle: " << target_heading_deg << " degrees (Diverting from Obstacle)" << endl;

    return 0;
}