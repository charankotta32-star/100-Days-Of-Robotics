#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Point2D {
    double x, y;
};

struct RRTNode {
    Point2D pos;
    int parent_idx;
};

struct CircleObstacle {
    Point2D center;
    double radius;
};

class RRTPlanner {
private:
    Point2D start;
    Point2D goal;
    double step_size;
    double goal_tolerance;
    int max_iterations;
    double arena_width, arena_height;
    vector<CircleObstacle> obstacles;
    vector<RRTNode> tree;

    default_random_engine rng;

    static double distance(const Point2D& a, const Point2D& b) {
        return hypot(a.x - b.x, a.y - b.y);
    }

    bool isCollisionFree(const Point2D& from, const Point2D& to) const {
        int steps = 10;
        for (int i = 0; i <= steps; ++i) {
            double t = (double)i / steps;
            Point2D p = {from.x + t * (to.x - from.x), from.y + t * (to.y - from.y)};
            for (const auto& obs : obstacles) {
                if (distance(p, obs.center) <= obs.radius) {
                    return false; // Collision detected
                }
            }
        }
        return true;
    }

    [[nodiscard]] int findNearestNode(const Point2D& target) const {
        int nearest_idx = 0;
        double min_dist = distance(tree[0].pos, target);
        for (size_t i = 1; i < tree.size(); ++i) {
            double d = distance(tree[i].pos, target);
            if (d < min_dist) {
                min_dist = d;
                nearest_idx = (int)i;
            }
        }
        return nearest_idx;
    }

public:
    RRTPlanner(Point2D s, Point2D g, double step, double tol, int max_iter, double w, double h)
        : start(s), goal(g), step_size(step), goal_tolerance(tol),
          max_iterations(max_iter), arena_width(w), arena_height(h) {
        tree.push_back({start, -1});
    }

    void addObstacle(Point2D center, double radius) {
        obstacles.push_back({center, radius});
    }

    bool plan(vector<Point2D>& out_path) {
        uniform_real_distribution<double> dist_x(0.0, arena_width);
        uniform_real_distribution<double> dist_y(0.0, arena_height);
        uniform_real_distribution<double> bias(0.0, 1.0);

        for (int iter = 0; iter < max_iterations; ++iter) {
            Point2D q_rand;
            // 10% Goal Biasing heuristic to accelerate tree convergence
            if (bias(rng) < 0.10) {
                q_rand = goal;
            } else {
                q_rand = {dist_x(rng), dist_y(rng)};
            }

            int near_idx = findNearestNode(q_rand);
            Point2D q_near = tree[near_idx].pos;

            // Steer by fixed step_size toward q_rand
            double theta = atan2(q_rand.y - q_near.y, q_rand.x - q_near.x);
            Point2D q_new = {q_near.x + step_size * cos(theta), q_near.y + step_size * sin(theta)};

            if (isCollisionFree(q_near, q_new)) {
                tree.push_back({q_new, near_idx});

                // Check if goal reached
                if (distance(q_new, goal) <= goal_tolerance) {
                    cout << "🎯 [RRT SUCCESS] Goal reached in " << iter + 1 << " iterations!\n";
                    // Reconstruct path
                    int curr = (int)tree.size() - 1;
                    while (curr != -1) {
                        out_path.push_back(tree[curr].pos);
                        curr = tree[curr].parent_idx;
                    }
                    reverse(out_path.begin(), out_path.end());
                    out_path.push_back(goal);
                    return true;
                }
            }
        }
        return false;
    }
};

int main() {
    cout << "--- DAY 43: RAPIDLY-EXPLORING RANDOM TREE (RRT) PLANNER ---" << endl << endl;

    Point2D start = {5.0, 5.0};
    Point2D goal  = {90.0, 90.0};

    // 100x100m arena, step size = 5.0m, goal tolerance = 5.0m, max 2000 iterations
    RRTPlanner rrt(start, goal, 5.0, 5.0, 2000, 100.0, 100.0);

    // Add obstacles blocking direct line of sight
    rrt.addObstacle({50.0, 50.0}, 15.0);
    rrt.addObstacle({30.0, 40.0}, 10.0);
    rrt.addObstacle({70.0, 60.0}, 12.0);

    vector<Point2D> planned_path;
    cout << fixed << setprecision(2);

    if (rrt.plan(planned_path)) {
        cout << "Generated Collision-Free Waypoint Path (" << planned_path.size() << " nodes):\n";
        cout << "Step |     Waypoint (X, Y)     | Segment Dist\n";
        cout << "----------------------------------------------\n";
        for (size_t i = 0; i < planned_path.size(); ++i) {
            double seg_d = (i == 0) ? 0.0 : hypot(planned_path[i].x - planned_path[i-1].x,
                                                 planned_path[i].y - planned_path[i-1].y);
            cout << " #" << setw(2) << (i + 1) << "  | ("
                 << setw(6) << planned_path[i].x << "m, "
                 << setw(6) << planned_path[i].y << "m) | "
                 << setw(5) << seg_d << "m\n";
        }
    } else {
        cout << "🚨 [RRT FAILED] Max iterations exceeded before reaching goal.\n";
    }

    return 0;
}