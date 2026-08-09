#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// BFS Traversal on a Robot Navigation Graph
void runBFS(int startNode, const vector<vector<int>>& adjList, int totalNodes) {
    vector<bool> visited(totalNodes, false);
    queue<int> q;

    visited[startNode] = true;
    q.push(startNode);

    cout << "BFS Autonomous Traversal Order: ";

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        cout << "Room " << current << " -> ";

        for (int neighbor : adjList[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    cout << "GOAL REACHED!" << endl;
}

int main() {
    cout << "--- DAY 9: BREADTH-FIRST SEARCH (BFS) PATHFINDING ---" << endl << endl;

    int totalRooms = 5;
    vector<vector<int>> mapAdjList(totalRooms);

    // Connecting rooms in the disaster maze
    mapAdjList[0] = {1, 2}; // Room 0 connects to Room 1 and 2
    mapAdjList[1] = {0, 3}; // Room 1 connects to 0 and 3
    mapAdjList[2] = {0, 4}; // Room 2 connects to 0 and 4
    mapAdjList[3] = {1};
    mapAdjList[4] = {2};

    // Start BFS from Entry Room 0
    runBFS(0, mapAdjList, totalRooms);

    return 0;
}