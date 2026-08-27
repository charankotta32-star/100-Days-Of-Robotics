#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

struct ObstacleNode {
    int obstacle_id;
    double distance_m;
    string zone_tag;
    ObstacleNode* left;
    ObstacleNode* right;

    ObstacleNode(int id, double dist, string zone)
        : obstacle_id(id), distance_m(dist), zone_tag(zone), left(nullptr), right(nullptr) {}
};

class ObstacleBST {
private:
    ObstacleNode* root;

    ObstacleNode* insertRecursive(ObstacleNode* node, int id, double dist, string zone) {
        if (!node) {
            return new ObstacleNode(id, dist, zone);
        }
        // Left subtree holds closer obstacles, Right subtree holds farther obstacles
        if (dist < node->distance_m) {
            node->left = insertRecursive(node->left, id, dist, zone);
        } else {
            node->right = insertRecursive(node->right, id, dist, zone);
        }
        return node;
    }

    // In-Order Traversal (Left -> Root -> Right) naturally prints sorted by distance
    void inorderRecursive(ObstacleNode* node) const {
        if (node) {
            inorderRecursive(node->left);
            cout << "  • [ID: #" << node->obstacle_id << "] Distance: "
                 << setw(4) << node->distance_m << "m (" << node->zone_tag << ")\n";
            inorderRecursive(node->right);
        }
    }

    void destroyTree(ObstacleNode* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    ObstacleBST() : root(nullptr) {}

    void insert(int id, double dist, string zone) {
        root = insertRecursive(root, id, dist, zone);
    }

    void displaySortedObstacles() const {
        cout << "--- SORTED OBSTACLE RANGE PROFILE (Closest -> Farthest) ---\n";
        if (!root) {
            cout << "No obstacles detected.\n";
            return;
        }
        inorderRecursive(root);
    }

    // Finds the absolute closest obstacle (Leftmost node in BST) in O(log N)
    void getClosestObstacle() const {
        if (!root) return;
        ObstacleNode* curr = root;
        while (curr->left != nullptr) {
            curr = curr->left;
        }
        cout << "\n🚨 [NEAREST THREAT DETECTED]: Obstacle #" << curr->obstacle_id
             << " at " << curr->distance_m << "m (" << curr->zone_tag << ")\n";
    }

    ~ObstacleBST() {
        destroyTree(root);
    }
};

int main() {
    cout << "--- DAY 27: BST SPATIAL RANGE INDEXER (DSA UNIT 3) ---" << endl << endl;

    ObstacleBST spatialIndex;
    cout << fixed << setprecision(2);

    // Range sensor telemetry stream inserting out-of-order obstacle pings
    spatialIndex.insert(101, 3.40, "Front_Center");
    spatialIndex.insert(102, 1.15, "Front_Left_Hazard");
    spatialIndex.insert(103, 5.20, "Far_Right");
    spatialIndex.insert(104, 0.85, "Critical_Bumper_Range");
    spatialIndex.insert(105, 2.70, "Left_Flank");

    spatialIndex.displaySortedObstacles();
    spatialIndex.getClosestObstacle();

    return 0;
}