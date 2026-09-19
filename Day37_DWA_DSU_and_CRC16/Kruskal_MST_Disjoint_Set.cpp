#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Edge {
    int src, dest, weight;

    // Sort edges in non-decreasing order of weight
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Disjoint Set Union (DSU) with Path Compression & Union by Rank
class DisjointSet {
private:
    vector<int> parent;
    vector<int> rank;

public:
    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    // Find with Path Compression: O(alpha(N)) ~ O(1)
    int find(int u) {
        if (u != parent[u]) {
            parent[u] = find(parent[u]);
        }
        return parent[u];
    }

    // Union by Rank: Prevents degenerate deep trees
    bool unite(int u, int v) {
        int root_u = find(u);
        int root_v = find(v);

        if (root_u == root_v) return false; // Cycle detected!

        if (rank[root_u] < rank[root_v]) {
            parent[root_u] = root_v;
        } else if (rank[root_u] > rank[root_v]) {
            parent[root_v] = root_u;
        } else {
            parent[root_v] = root_u;
            rank[root_u]++;
        }
        return true;
    }
};

class KruskalMST {
private:
    int V;
    vector<Edge> edges;

public:
    KruskalMST(int vertices) : V(vertices) {}

    void addEdge(int u, int v, int w) {
        edges.push_back({u, v, w});
    }

    void computeMST() {
        // Step 1: Sort all edges in non-decreasing order of cost: O(E log E)
        sort(edges.begin(), edges.end());

        DisjointSet dsu(V);
        vector<Edge> mst_edges;
        int total_weight = 0;

        // Step 2: Pick the smallest edge. If it doesn't form a cycle, include it.
        for (const auto& edge : edges) {
            if (dsu.unite(edge.src, edge.dest)) {
                mst_edges.push_back(edge);
                total_weight += edge.weight;
                if ((int)mst_edges.size() == V - 1) break; // MST complete
            }
        }

        cout << "--- KRUSKAL'S MINIMUM SPANNING TREE (MST) EDGES ---\n";
        cout << "Edge Wire | Source Node <---> Target Node | Weight/Cost\n";
        cout << "---------------------------------------------------------\n";
        for (const auto& e : mst_edges) {
            cout << "   #" << setw(2) << &e - &mst_edges[0] + 1 << "   |      "
                 << setw(3) << e.src << "    <--->    "
                 << setw(3) << e.dest << "    |     "
                 << setw(4) << e.weight << "\n";
        }
        cout << "---------------------------------------------------------\n";
        cout << ">>> Total Optimal MST Network Cost: " << total_weight << " units <<<\n";
    }
};

int main() {
    cout << "--- DAY 37: KRUSKAL'S ALGORITHM WITH DSU (DSA UNIT 5) ---" << endl << endl;

    // Mesh sensor network with 6 nodes (Zones 0 to 5)
    KruskalMST network(6);

    network.addEdge(0, 1, 4);
    network.addEdge(0, 2, 4);
    network.addEdge(1, 2, 2);
    network.addEdge(1, 3, 3);
    network.addEdge(2, 3, 1);
    network.addEdge(2, 4, 6);
    network.addEdge(3, 4, 6);
    network.addEdge(3, 5, 2);
    network.addEdge(4, 5, 3);

    network.computeMST();

    return 0;
}