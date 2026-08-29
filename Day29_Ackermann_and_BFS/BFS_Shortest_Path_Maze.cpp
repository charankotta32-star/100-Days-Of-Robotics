#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class MazeBFS {
private:
    int num_zones;
    vector<vector<int>> adj_list;

public:
    MazeBFS(int nodes) : num_zones(nodes) {
        adj_list.resize(nodes);
    }

    // Add bidirectional path between two zones
    void addPath(int u, int v) {
        adj_list[u].push_back(v);
        adj_list[v].push_back(u);
    }

    // BFS Shortest Path using a Queue (FIFO)
    void findShortestPath(int start_node, int target_node) {
        vector<bool> visited(num_zones, false);
        vector<int> parent(num_zones, -1); // To reconstruct the path
        queue<int> q;

        // Initialize Start Node
        q.push(start_node);
        visited[start_node] = true;

        bool found = false;
        cout << ">>> INITIATING BFS SCAN FROM ZONE " << start_node << " <<<" << endl;

        while (!q.empty()) {
            int current = q.front();
            q.pop();
            cout << " -> Scanning Zone [" << current << "]" << endl;

            if (current == target_node) {
                found = true;
                break;
            }

            // Explore all immediate neighbors (Breadth-First)
            for (int neighbor : adj_list[current]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    parent[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }

        if (found) {
            cout << "\n✅ TARGET ZONE [" << target_node << "] REACHED!" << endl;
            cout << "Shortest Path Reconstruction (Reverse): ";
            int crawl = target_node;
            while (crawl != -1) {
                cout << crawl << (crawl == start_node ? "" : " <- ");
                crawl = parent[crawl];
            }
            cout << endl;
        } else {
            cout << "\n🚨 TARGET UNREACHABLE!" << endl;
        }
    }
};

int main() {
    cout << "--- DAY 29: BFS SHORTEST PATH MAZE SOLVER (DSA UNIT 4) ---" << endl << endl;

    MazeGraph roverMaze(6);

    // Arena connections
    roverMaze.addPath(0, 1);
    roverMaze.addPath(0, 2);
    roverMaze.addPath(1, 3);
    roverMaze.addPath(2, 4);
    roverMaze.addPath(3, 5); // Connects to Goal
    roverMaze.addPath(4, 5); // Connects to Goal

    // Find shortest path from Start (0) to Goal (5)
    roverMaze.findShortestPath(0, 5);

    return 0;
}