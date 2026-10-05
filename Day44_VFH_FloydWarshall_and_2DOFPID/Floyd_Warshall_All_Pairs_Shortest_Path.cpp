#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

class FloydWarshall {
private:
    int V;
    vector<vector<int>> dist;
    vector<vector<int>> next_node;
    const int INF = 1e7;

public:
    explicit FloydWarshall(int vertices) : V(vertices) {
        dist.assign(V, vector<int>(V, INF));
        next_node.assign(V, vector<int>(V, -1));

        for (int i = 0; i < V; ++i) {
            dist[i][i] = 0;
            next_node[i][i] = i;
        }
    }

    void addEdge(int u, int v, int weight) {
        dist[u][v] = weight;
        next_node[u][v] = v;
    }

    bool computeAllPairsShortestPaths() {
        // Dynamic Programming: Try intermediate vertex k for all pairs (i, j)
        for (int k = 0; k < V; ++k) {
            for (int i = 0; i < V; ++i) {
                for (int j = 0; j < V; ++j) {
                    if (dist[i][k] != INF && dist[k][j] != INF) {
                        if (dist[i][k] + dist[k][j] < dist[i][j]) {
                            dist[i][j] = dist[i][k] + dist[k][j];
                            next_node[i][j] = next_node[i][k];
                        }
                    }
                }
            }
        }

        // Negative cycle check: if distance from any node to itself becomes negative
        for (int i = 0; i < V; ++i) {
            if (dist[i][i] < 0) {
                cout << "🚨 [NEGATIVE CYCLE DETECTED] Infinite negative cycle involving node " << i << "!\n";
                return false;
            }
        }
        return true;
    }

    void printDistanceMatrix() const {
        cout << "--- ALL-PAIRS SHORTEST DISTANCE MATRIX ---\n     ";
        for (int i = 0; i < V; ++i) cout << "  [" << i << "] ";
        cout << "\n   +" << string(V * 6, '-') << "\n";

        for (int i = 0; i < V; ++i) {
            cout << " [" << i << "] | ";
            for (int j = 0; j < V; ++j) {
                if (dist[i][j] == INF) cout << " INF ";
                else cout << setw(4) << dist[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    void printPath(int u, int v) const {
        if (dist[u][v] == INF) {
            cout << "No path exists between " << u << " and " << v << ".\n";
            return;
        }
        cout << "Optimal Route from [" << u << "] ➔ [" << v << "] (Cost: " << dist[u][v] << "): ";
        int curr = u;
        cout << curr;
        while (curr != v) {
            curr = next_node[curr][v];
            cout << " ➔ " << curr;
        }
        cout << "\n";
    }
};

int main() {
    cout << "--- DAY 44: FLOYD-WARSHALL ALGORITHM (DSA UNIT 5) ---" << endl << endl;

    // 4-Node Arena Network
    FloydWarshall fw(4);

    fw.addEdge(0, 1, 5);
    fw.addEdge(0, 3, 10);
    fw.addEdge(1, 2, 3);
    fw.addEdge(2, 3, 1);
    fw.addEdge(3, 0, 2);

    if (fw.computeAllPairsShortestPaths()) {
        fw.printDistanceMatrix();
        fw.printPath(0, 2);
        fw.printPath(0, 3);
        fw.printPath(1, 0);
    }

    return 0;
}