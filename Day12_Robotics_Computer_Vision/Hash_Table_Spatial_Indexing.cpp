#include <iostream>
#include <vector>

using namespace std;

const int TABLE_SIZE = 7;

// Hash Table Node for 2D Obstacle Map
struct HashNode {
    int x_coord;
    int y_coord;
    string obstacle_type;
    HashNode* next;
};

class SpatialHashTable {
private:
    HashNode* table[TABLE_SIZE];

    // Simple Hash Function: (X * 31 + Y) % TABLE_SIZE
    int hashFunction(int x, int y) {
        return (x * 31 + y) % TABLE_SIZE;
    }

public:
    SpatialHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
    }

    void insertObstacle(int x, int y, string type) {
        int index = hashFunction(x, y);
        HashNode* new_node = new HashNode{x, y, type, table[index]};
        table[index] = new_node; // Separate Chaining for Collisions
        cout << "[HASH MAP] Inserted " << type << " at (" << x << ", " << y << ") -> Hash Index " << index << endl;
    }

    void searchObstacle(int x, int y) {
        int index = hashFunction(x, y);
        HashNode* temp = table[index];
        while (temp != nullptr) {
            if (temp->x_coord == x && temp->y_coord == y) {
                cout << "[FOUND] Obstacle at (" << x << ", " << y << "): " << temp->obstacle_type << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "[CLEAR] No obstacle logged at (" << x << ", " << y << ")." << endl;
    }
};

int main() {
    cout << "--- DAY 12: SPATIAL HASH TABLE LOOKUP (DSA UNIT 4) ---" << endl << endl;

    SpatialHashTable mapHash;

    mapHash.insertObstacle(4, 2, "Concrete Pillar");
    mapHash.insertObstacle(1, 5, "Debris Block");

    cout << endl;
    mapHash.searchObstacle(4, 2);
    mapHash.searchObstacle(3, 3);

    return 0;
}