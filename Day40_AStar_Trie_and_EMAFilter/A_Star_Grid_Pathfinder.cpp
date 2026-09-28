#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

const int ROWS = 6;
const int COLS = 6;

struct Node {
    int r, c;
    int g_cost;
    int h_cost;
    int f_cost;
    int parent_r, parent_c;

    bool operator>(const Node& other) const {
        return f_cost > other.f_cost;
    }
};

class AStarGridPlanner {
private:
    // 4-directional motion: Up, Down, Left, Right
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    static int calculateManhattan(int r, int c, int goal_r, int goal_c) {
        return abs(r - goal_r) + abs(c - goal_c);
    }

public:
    void planPath(const vector<vector<int>>& grid, pair<int, int> start, pair<int, int> goal) {
        priority_queue<Node, vector<Node>, greater<Node>> open_set;
        vector<vector<bool>> closed_set(ROWS, vector<bool>(COLS, false));
        vector<vector<pair<int, int>>> parent(ROWS, vector<pair<int, int>>(COLS, {-1, -1}));
        vector<vector<int>> g_scores(ROWS, vector<int>(COLS, 1e9));

        int start_h = calculateManhattan(start.first, start.second, goal.first, goal.second);
        open_set.push({start.first, start.second, 0, start_h, start_h, -1, -1});
        g_scores[start.first][start.second] = 0;

        bool reached = false;

        while (!open_set.empty()) {
            Node current = open_set.top();
            open_set.pop();

            int r = current.r;
            int c = current.c;

            if (closed_set[r][c]) continue;
            closed_set[r][c] = true;

            // Target check
            if (r == goal.first && c == goal.second) {
                reached = true;
                break;
            }

            // Expand 4 neighbors
            for (int i = 0; i < 4; i++) {
                int nr = r + dr[i];
                int nc = c + dc[i];

                // Bounds & obstacle check (0 = Free, 1 = Obstacle)
                if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && grid[nr][nc] == 0 && !closed_set[nr][nc]) {
                    int tentative_g = g_scores[r][c] + 1; // Step cost = 1

                    if (tentative_g < g_scores[nr][nc]) {
                        g_scores[nr][nc] = tentative_g;
                        parent[nr][nc] = {r, c};
                        int h = calculateManhattan(nr, nc, goal.first, goal.second);
                        open_set.push({nr, nc, tentative_g, h, tentative_g + h, r, c});
                    }
                }
            }
        }

        if (!reached) {
            cout << "🚨 [PATHFINDER] No valid path to goal found (Trapped by Obstacles)!\n";
            return;
        }

        // Reconstruct Path
        vector<pair<int, int>> path;
        pair<int, int> curr = goal;
        while (curr != start) {
            path.push_back(curr);
            curr = parent[curr.first][curr.second];
        }
        path.push_back(start);
        reverse(path.begin(), path.end());

        cout << "--- A* OPTIMAL TRAJECTORY FOUND (Steps: " << path.size() - 1 << ") ---\n";
        for (size_t i = 0; i < path.size(); i++) {
            cout << "(" << path[i].first << ", " << path[i].second << ")";
            if (i < path.size() - 1) cout << " ➔ ";
        }
        cout << "\n\nGrid Path Visualization:\n";

        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                if (r == start.first && c == start.second) cout << " S ";
                else if (r == goal.first && c == goal.second) cout << " G ";
                else if (grid[r][c] == 1) cout << " ■ "; // Obstacle
                else {
                    bool on_path = false;
                    for (const auto& p : path) {
                        if (p.first == r && p.second == c) { on_path = true; break; }
                    }
                    cout << (on_path ? " * " : " . ");
                }
            }
            cout << "\n";
        }
    }
};

int main() {
    cout << "--- DAY 40: A* GRID PATHFINDER (ROBOTICS NAVIGATION) ---" << endl << endl;

    // 6x6 Arena Map: 0 = Free space, 1 = Solid obstacle wall
    vector<vector<int>> arena_map = {
        {0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 0}, // Wall blocking direct path
        {0, 0, 0, 0, 1, 0},
        {1, 1, 1, 0, 1, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 0}
    };

    AStarGridPlanner planner;
    pair<int, int> start_pos = {0, 0};
    pair<int, int> goal_pos  = {4, 3};

    planner.planPath(arena_map, start_pos, goal_pos);

    return 0;
}