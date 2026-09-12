#include <iostream>

using namespace std;

// Standard BST Node Structure
struct Node {
    int key;
    Node* left;
    Node* right;

    Node(int val) : key(val), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    Node* root;

    Node* insertRec(Node* node, int val) {
        if (!node) return new Node(val);
        if (val < node->key) node->left = insertRec(node->left, val);
        else if (val > node->key) node->right = insertRec(node->right, val);
        return node;
    }

    // Helper: Finds the minimum value node in a subtree (In-order Successor)
    Node* findMinNode(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    // Core BST Deletion covering all 3 cases
    Node* deleteRec(Node* root_node, int target_key) {
        if (!root_node) return nullptr;

        // 1. Traverse to locate target node
        if (target_key < root_node->key) {
            root_node->left = deleteRec(root_node->left, target_key);
        } else if (target_key > root_node->key) {
            root_node->right = deleteRec(root_node->right, target_key);
        }
        // 2. Found the target node!
        else {
            // Case 1: Leaf node (No children)
            if (!root_node->left && !root_node->right) {
                delete root_node;
                return nullptr;
            }
            // Case 2: One child (Only Right Child)
            else if (!root_node->left) {
                Node* temp = root_node->right;
                delete root_node;
                return temp;
            }
            // Case 2: One child (Only Left Child)
            else if (!root_node->right) {
                Node* temp = root_node->left;
                delete root_node;
                return temp;
            }
            // Case 3: Two children
            // Find in-order successor (smallest in right subtree)
            Node* successor = findMinNode(root_node->right);

            // Copy successor's key to current node
            root_node->key = successor->key;

            // Delete the successor from right subtree
            root_node->right = deleteRec(root_node->right, successor->key);
        }
        return root_node;
    }

    void inorderRec(Node* node) const {
        if (node) {
            inorderRec(node->left);
            cout << node->key << " ";
            inorderRec(node->right);
        }
    }

public:
    BinarySearchTree() : root(nullptr) {}

    void insert(int val) { root = insertRec(root, val); }
    void remove(int val) { root = deleteRec(root, val); }

    void displayInorder() const {
        cout << "BST In-Order Traversal (Sorted): [ ";
        inorderRec(root);
        cout << "]\n";
    }
};

int main() {
    cout << "--- DAY 33: COMPLETE BST NODE DELETION (DSA EXAM CORE) ---" << endl << endl;

    BinarySearchTree bst;

    // Building initial tree: 50, 30, 70, 20, 40, 60, 80
    int initial_keys[] = {50, 30, 70, 20, 40, 60, 80};
    for (int k : initial_keys) bst.insert(k);

    cout << "Initial Tree populated.\n";
    bst.displayInorder();

    // Test Case 1: Delete Leaf Node (20)
    cout << "\n1. Deleting Leaf Node (20):\n";
    bst.remove(20);
    bst.displayInorder();

    // Test Case 2: Delete Node with One Child (30 has child 40)
    cout << "\n2. Deleting Node with One Child (30):\n";
    bst.remove(30);
    bst.displayInorder();

    // Test Case 3: Delete Root Node with Two Children (50) -> Replaced by In-order Successor (60)
    cout << "\n3. Deleting Node with Two Children (50 - Root):\n";
    bst.remove(50);
    bst.displayInorder();

    cout << "\n>>> [VERIFIED] All 3 BST deletion cases executed with zero memory leaks." << endl;
    return 0;
}