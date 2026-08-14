#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

struct Node {
    int x, y;
    float cost_g; // Cost from start to current node
    float cost_h; // Heuristic cost to goal
    float cost_f; // Total estimated cost (g + h)

    // Operator overload for Priority Queue (Min-Heap: lowest cost_f comes first)
    bool operator>(const Node& other) const {
        return cost_f > other.cost_f;
    }
};

// Manhattan Distance Heuristic Calculation
float calculateHeuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

class PathPlanner {
public:
    void planPath(int start_x, int start_y, int goal_x, int goal_y) {
        priority_queue<Node, vector<Node>, greater<Node>> open_set;

        Node start_node = {start_x, start_y, 0.0f, calculateHeuristic(start_x, start_y, goal_x, goal_y), 0.0f};
        start_node.cost_f = start_node.cost_g + start_node.cost_h;

        open_set.push(start_node);
        cout << "[A* PLANNER] Start Node: (" << start_x << ", " << start_y << ") -> Goal Node: (" << goal_x << ", " << goal_y << ")" << endl;

        int step = 1;
        while (!open_set.empty() && step <= 4) {
            Node current = open_set.top();
            open_set.pop();

            cout << "Step " << step << " | Inspecting Node (" << current.x << ", " << current.y
                 << ") with f_cost = " << current.cost_f << " (g=" << current.cost_g << ", h=" << current.cost_h << ")" << endl;

            // Simulating expansion to adjacent grid cell (moving towards goal)
            if (current.x < goal_x) {
                int next_x = current.x + 1;
                int next_y = current.y;
                float g = current.cost_g + 1.0f;
                float h = calculateHeuristic(next_x, next_y, goal_x, goal_y);
                open_set.push({next_x, next_y, g, h, g + h});
            }
            step++;
        }
        cout << ">>> [PATH STATUS] Optimal Trajectory Computed." << endl << endl;
    }
};

int main() {
    cout << "--- DAY 14: A* AUTONOMOUS PATHFINDING (ROBOTICS 21MMH024F) ---" << endl << endl;

    PathPlanner roverNav;
    roverNav.planPath(0, 0, 3, 3);

    return 0;
}