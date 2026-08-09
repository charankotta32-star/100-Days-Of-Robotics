#include <iostream>
#include <vector>

using namespace std;

// Representing a Grid/Maze Map using a Graph Adjacency Matrix
class MazeGraph {
private:
    int num_vertices;
    vector<vector<int>> adjMatrix;

public:
    MazeGraph(int nodes) : num_vertices(nodes), adjMatrix(nodes, vector<int>(nodes, 0)) {}

    // Add an edge (path) between room u and room v
    void addPath(int u, int v) {
        adjMatrix[u][v] = 1;
        adjMatrix[v][u] = 1; // Undirected path
    }

    void printMap() {
        cout << "Maze Adjacency Matrix (1 = Path Available, 0 = Wall Blocked):" << endl;
        for (int i = 0; i < num_vertices; i++) {
            cout << "Room " << i << ": [ ";
            for (int j = 0; j < num_vertices; j++) {
                cout << adjMatrix[i][j] << " ";
            }
            cout << "]" << endl;
        }
    }
};

int main() {
    cout << "--- DAY 9: GRAPH ADJACENCY MATRIX (ROBORUSH MAZE) ---" << endl << endl;

    MazeGraph robotMap(4); // 4 Rooms in the maze

    robotMap.addPath(0, 1); // Room 0 connects to Room 1
    robotMap.addPath(1, 2); // Room 1 connects to Room 2
    robotMap.addPath(2, 3); // Room 2 connects to Room 3

    robotMap.printMap();

    return 0;
}