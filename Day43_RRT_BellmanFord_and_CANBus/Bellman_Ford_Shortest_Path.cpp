#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

struct GraphEdge {
    int src;
    int dest;
    int weight;
};

class BellmanFord {
private:
    int V; // Vertices
    int E; // Edges
    vector<GraphEdge> edges;
    const int INF = 1e8;

public:
    BellmanFord(int v_count, int e_count) : V(v_count), E(e_count) {
        edges.reserve(E);
    }

    void addEdge(int u, int v, int w) {
        edges.push_back({u, v, w});
    }

    void findShortestPath(int src) {
        vector<int> dist(V, INF);
        vector<int> parent(V, -1);
        dist[src] = 0;

        // Step 1: Relax all edges strictly (V - 1) times: O(V * E)
        for (int i = 1; i <= V - 1; ++i) {
            for (const auto& edge : edges) {
                if (dist[edge.src] != INF && dist[edge.src] + edge.weight < dist[edge.dest]) {
                    dist[edge.dest] = dist[edge.src] + edge.weight;
                    parent[edge.dest] = edge.src;
                }
            }
        }

        // Step 2: 1-Pass Negative Cycle Check
        bool has_negative_cycle = false;
        for (const auto& edge : edges) {
            if (dist[edge.src] != INF && dist[edge.src] + edge.weight < dist[edge.dest]) {
                has_negative_cycle = true;
                break;
            }
        }

        if (has_negative_cycle) {
            cout << "🚨 [CRITICAL ALERT] Negative-weight cycle detected! Minimum distance is undefined.\n";
            return;
        }

        cout << "--- BELLMAN-FORD SHORTEST PATHS FROM SOURCE [" << src << "] ---\n";
        cout << "Target Node | Shortest Distance | Reconstructed Optimal Route\n";
        cout << "------------------------------------------------------------\n";
        for (int i = 0; i < V; ++i) {
            cout << "   Node " << i << "   |       " << setw(5);
            if (dist[i] == INF) cout << "INF";
            else cout << dist[i];
            cout << "       | ";

            if (dist[i] == INF) {
                cout << "Unreachable\n";
                continue;
            }

            vector<int> path;
            for (int at = i; at != -1; at = parent[at]) {
                path.push_back(at);
            }
            reverse(path.begin(), path.end());

            for (size_t p = 0; p < path.size(); ++p) {
                cout << path[p] << (p + 1 == path.size() ? "" : " ➔ ");
            }
            cout << "\n";
        }
    }
};

int main() {
    cout << "--- DAY 43: BELLMAN-FORD ALGORITHM (DSA UNIT 5) ---" << endl << endl;

    // 5-Node Network with a negative cost edge (e.g. regenerative braking gradient)
    BellmanFord graph(5, 7);

    graph.addEdge(0, 1, 6);
    graph.addEdge(0, 2, 7);
    graph.addEdge(1, 2, 8);
    graph.addEdge(1, 3, -4); // Negative weight edge!
    graph.addEdge(1, 4, -2); // Negative weight edge!
    graph.addEdge(2, 3, 9);
    graph.addEdge(3, 4, 7);

    graph.findShortestPath(0);

    return 0;
}