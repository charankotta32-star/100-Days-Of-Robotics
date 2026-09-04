#include <iostream>
#include <queue>
#include <string>

using namespace std;

struct TreeNode {
    int zone_id;
    string zone_name;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int id, string name) : zone_id(id), zone_name(name), left(nullptr), right(nullptr) {}
};

class ArenaSpatialTree {
public:
    // Breadth-First Level-Order Traversal using Queue (O(N) Time, O(W) Space)
    static void printLevelOrder(TreeNode* root) {
        if (!root) {
            cout << "Tree is empty.\n";
            return;
        }

        queue<TreeNode*> q;
        q.push(root);

        int level = 1;
        cout << "--- HIERARCHICAL SPATIAL LEVEL-ORDER TRAVERSAL ---\n";

        while (!q.empty()) {
            int nodes_in_current_level = q.size();
            cout << "Level " << level << ": ";

            for (int i = 0; i < nodes_in_current_level; i++) {
                TreeNode* current = q.front();
                q.pop();

                cout << "[" << current->zone_id << ": " << current->zone_name << "] ";

                if (current->left)  q.push(current->left);
                if (current->right) q.push(current->right);
            }
            cout << endl;
            level++;
        }
    }

    static void freeTree(TreeNode* root) {
        if (root) {
            freeTree(root->left);
            freeTree(root->right);
            delete root;
        }
    }
};

int main() {
    cout << "--- DAY 32: BINARY TREE LEVEL-ORDER TRAVERSAL (DSA UNIT 3) ---" << endl << endl;

    // Constructing arena hierarchical navigation tree
    //                 [1: Arena_Master]
    //                   /           \
    //      [2: Zone_North]        [3: Zone_South]
    //         /        \              /        \
    //   [4: Ramp]  [5: Bridge]  [6: Pit]  [7: Goal]

    TreeNode* root = new TreeNode(1, "Arena_Master");
    root->left  = new TreeNode(2, "Zone_North");
    root->right = new TreeNode(3, "Zone_South");

    root->left->left   = new TreeNode(4, "Ramp_ClimbX");
    root->left->right  = new TreeNode(5, "Bridge_Odyssey");
    root->right->left  = new TreeNode(6, "Pit_Hazard");
    root->right->right = new TreeNode(7, "Goal_Striker");

    ArenaSpatialTree::printLevelOrder(root);

    ArenaSpatialTree::freeTree(root);
    return 0;
}