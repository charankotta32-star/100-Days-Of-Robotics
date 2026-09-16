#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>

using namespace std;

// Pair representing: (Distance, Vertex)
typedef pair<int, int> iPair;

class WeightedArenaGraph {
private:
    int V; // Number of vertices / zones
    // Adjacency list: adj[u] = list of (v, weight)
    vector<vector<pair<int, int>>> adj;

public:
    WeightedArenaGraph(int vertices) : V(vertices) {
        adj.resize(vertices);
    }

    void addEdge(int u, int v, int weight) {
        adj[u].push_back({v, weight});
        adj[v].push_back({u, weight}); // Undirected track
    }

    void findShortestPaths(int src) {
        // Min-Priority Queue storing pairs of (distance, vertex)
        priority_queue<iPair, vector<iPair>, greater<iPair>> pq;

        // Initialize distances to infinity
        const int INF = 1e9;
        vector<int> dist(V, INF);
        vector<int> parent(V, -1);

        // Distance to source is 0
        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            int u = pq.top().second;
            int d = pq.top().first;
            pq.pop();

            // Skip stale pairs
            if (d > dist[u]) continue;

            // Relax neighbor edges
            for (auto edge : adj[u]) {
                int v = edge.first;
                int weight = edge.second;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        cout << "--- DIJKSTRA SHORTEST PATHS FROM START ZONE [" << src << "] ---\n";
        cout << "Target Zone | Minimum Cost/Distance | Optimal Waypoint Path\n";
        cout << "-----------------------------------------------------------\n";

        for (int i = 0; i < V; i++) {
            cout << "   Zone " << i << "   |         " << setw(4) << dist[i] << "         | ";

            // Reconstruct path
            int crawl = i;
            vector<int> path;
            while (crawl != -1) {
                path.push_back(crawl);
                crawl = parent[crawl];
            }
            for (int p = path.size() - 1; p >= 0; p--) {
                cout << path[p] << (p == 0 ? "" : " -> ");
            }
            cout << endl;
        }
    }
};

int main() {
    cout << "--- DAY 36: DIJKSTRA SHORTEST PATH ALGORITHM (DSA CORE) ---" << endl << endl;

    // Arena map with 6 distinct tactical zones
    WeightedArenaGraph arena(6);

    // Adding weighted terrain costs (representing surface friction / distance)
    arena.addEdge(0, 1, 4);
    arena.addEdge(0, 2, 2);
    arena.addEdge(1, 2, 1);
    arena.addEdge(1, 3, 5);
    arena.addEdge(2, 3, 8);
    arena.addEdge(2, 4, 10);
    arena.addEdge(3, 4, 2);
    arena.addEdge(3, 5, 6);
    arena.addEdge(4, 5, 3);

    // Compute shortest navigation routes from Start (Zone 0)
    arena.findShortestPaths(0);

    return 0;
}