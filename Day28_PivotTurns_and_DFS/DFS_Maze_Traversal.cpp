#include <iostream>
#include <vector>

using namespace std;

class MazeGraph {
private:
    int num_zones;
    vector<vector<int>> adjacency_matrix;
    vector<bool> visited;

    void dfsRecursive(int current_zone) {
        visited[current_zone] = true;
        cout << " -> Exploring Maze Zone [" << current_zone << "]" << endl;

        // Explore all connected adjacent zones
        for (int neighbor = 0; neighbor < num_zones; neighbor++) {
            if (adjacency_matrix[current_zone][neighbor] == 1 && !visited[neighbor]) {
                cout << "    Path found from Zone " << current_zone << " to Zone " << neighbor << endl;
                dfsRecursive(neighbor);
            }
        }
        cout << " <- Backtracking from Zone [" << current_zone << "] (Dead End / Fully Explored)" << endl;
    }

public:
    MazeGraph(int nodes) : num_zones(nodes) {
        adjacency_matrix.resize(nodes, vector<int>(nodes, 0));
        visited.resize(nodes, false);
    }

    void addPath(int u, int v) {
        adjacency_matrix[u][v] = 1;
        adjacency_matrix[v][u] = 1; // Undirected paths
    }

    void exploreMaze(int start_zone) {
        cout << ">>> INITIATING DFS MAZE TRAVERSAL FROM ZONE " << start_zone << " <<<" << endl;
        fill(visited.begin(), visited.end(), false); // Reset visited array
        dfsRecursive(start_zone);
        cout << ">>> MAZE EXPLORATION COMPLETE <<<" << endl << endl;
    }
};

int main() {
    cout << "--- DAY 28: DFS AUTONOMOUS MAZE TRAVERSAL (DSA UNIT 4) ---" << endl << endl;

    // Arena map with 6 distinct physical zones
    MazeGraph arena(6);

    // Map the connections (0 = Start, 5 = Exit)
    arena.addPath(0, 1);
    arena.addPath(0, 2);
    arena.addPath(1, 3);
    arena.addPath(2, 4);
    arena.addPath(3, 5); // Connects to exit
    arena.addPath(4, 5); // Connects to exit

    // Robot begins exploration from Zone 0
    arena.exploreMaze(0);

    return 0;
}