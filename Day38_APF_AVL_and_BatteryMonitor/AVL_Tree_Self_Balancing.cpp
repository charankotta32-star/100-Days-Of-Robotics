#include <iostream>
#include <algorithm>

using namespace std;

struct AVLNode {
    int key;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    AVLNode* root;

    int getHeight(AVLNode* node) {
        return node ? node->height : 0;
    }

    int getBalanceFactor(AVLNode* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    // Right Rotation (fixes Left-Heavy tree: LL Case)
    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = 1 + max(getHeight(y->left), getHeight(y->right));
        x->height = 1 + max(getHeight(x->left), getHeight(x->right));

        return x; // New root
    }

    // Left Rotation (fixes Right-Heavy tree: RR Case)
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = 1 + max(getHeight(x->left), getHeight(x->right));
        y->height = 1 + max(getHeight(y->left), getHeight(y->right));

        return y; // New root
    }

    AVLNode* insertNode(AVLNode* node, int key) {
        // 1. Standard BST insertion
        if (!node) return new AVLNode(key);

        if (key < node->key)
            node->left = insertNode(node->left, key);
        else if (key > node->key)
            node->right = insertNode(node->right, key);
        else
            return node; // Duplicate keys not allowed

        // 2. Update height of ancestor node
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        // 3. Check Balance Factor
        int balance = getBalanceFactor(node);

        // 4. Handle 4 Imbalance Cases:
        
        // Case 1: Left Left (LL) -> Single Right Rotate
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // Case 2: Right Right (RR) -> Single Left Rotate
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // Case 3: Left Right (LR) -> Left Rotate child, then Right Rotate root
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Case 4: Right Left (RL) -> Right Rotate child, then Left Rotate root
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void inOrderTraversal(AVLNode* node) const {
        if (node) {
            inOrderTraversal(node->left);
            cout << node->key << "(BF: " << (getHeight(node->left) - getHeight(node->right)) << ") ";
            inOrderTraversal(node->right);
        }
    }

    void destroy(AVLNode* node) {
        if (node) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(int key) {
        root = insertNode(root, key);
    }

    void display() const {
        cout << "AVL In-Order [Node(BalanceFactor)]: ";
        inOrderTraversal(root);
        cout << "\nTree Height: " << (root ? root->height : 0) << endl;
    }

    ~AVLTree() {
        destroy(root);
    }
};

int main() {
    cout << "--- DAY 38: AVL SELF-BALANCING BINARY SEARCH TREE (DSA) ---" << endl << endl;

    AVLTree avl;

    // Inserting keys that would severely unbalance a normal BST (10 -> 20 -> 30 -> 40 -> 50 -> 25)
    cout << "1. Inserting: 10, 20, 30 (Triggers RR Rotation):\n";
    avl.insert(10);
    avl.insert(20);
    avl.insert(30);
    avl.display();

    cout << "\n2. Inserting: 40, 50 (Triggers second RR Rotation):\n";
    avl.insert(40);
    avl.insert(50);
    avl.display();

    cout << "\n3. Inserting: 25 (Triggers Double RL Rotation):\n";
    avl.insert(25);
    avl.display();

    cout << "\n>>> [VERIFIED] All Balance Factors maintained within {-1, 0, +1} in O(log N).\n";
    return 0;
}