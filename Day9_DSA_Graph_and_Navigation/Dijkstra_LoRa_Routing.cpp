#include <iostream>
#include <vector>
#include <climits>

using namespace std;

#define V 4 // 4 LoRa Breadcrumb Nodes in the disaster building

// Dijkstra's Shortest Path Algorithm for LoRa Packet Routing
int findMinDistanceNode(const vector<int>& dist, const vector<bool>& visited) {
    int minVal = INT_MAX, minIndex = -1;
    for (int v = 0; v < V; v++) {
        if (!visited[v] && dist[v] <= minVal) {
            minVal = dist[v];
            minIndex = v;
        }
    }
    return minIndex;
}

void dijkstraLoRaRouting(int graph[V][V], int sourceNode) {
    vector<int> dist(V, INT_MAX);
    vector<bool> visited(V, false);

    dist[sourceNode] = 0; // Distance to itself is 0

    for (int count = 0; count < V - 1; count++) {
        int u = findMinDistanceNode(dist, visited);
        if (u == -1) break;
        visited[u] = true;

        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                }
        }
    }

    cout << "Optimal LoRa Packet Routes from Source Node " << sourceNode << ":" << endl;
    for (int i = 0; i < V; i++) {
        cout << " -> To Node " << i << ": Total Latency = " << dist[i] << " ms" << endl;
    }
}

int main() {
    cout << "--- DAY 9: DIJKSTRA'S LORA MESH ROUTING ALGORITHM ---" << endl << endl;

    // Graph matrix representing signal latency (ms) between LoRa nodes
    int loraMeshGraph[V][V] = {
        {0, 10, 20, 0},
        {10, 0, 5, 30},
        {20, 5, 0, 15},
        {0, 30, 15, 0}
    };

    dijkstraLoRaRouting(loraMeshGraph, 0);

    return 0;
}