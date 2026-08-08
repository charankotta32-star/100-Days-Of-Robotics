#include <iostream>

using namespace std;

struct BSTNode {
    float obstacle_distance;
    BSTNode* left;
    BSTNode* right;
};

// BST Insertion: Left < Root < Right
BSTNode* insertBST(BSTNode* root, float distance) {
    if (root == nullptr) {
        return new BSTNode{distance, nullptr, nullptr};
    }
    if (distance < root->obstacle_distance) {
        root->left = insertBST(root->left, distance);
    } else {
        root->right = insertBST(root->right, distance);
    }
    return root;
}

// Inorder Traversal prints BST elements in SORTED order!
void printSortedObstacles(BSTNode* root) {
    if (root == nullptr) return;
    printSortedObstacles(root->left);
    cout << root->obstacle_distance << "m  ";
    printSortedObstacles(root->right);
}

int main() {
    cout << "--- DAY 8: BST OBSTACLE DISTANCE SORTING ---" << endl << endl;

    BSTNode* obstacleTree = nullptr;

    // Inserting random radar pings
    obstacleTree = insertBST(obstacleTree, 4.2);
    obstacleTree = insertBST(obstacleTree, 1.5);
    obstacleTree = insertBST(obstacleTree, 3.8);
    obstacleTree = insertBST(obstacleTree, 0.8); // Closest obstacle!

    cout << "Obstacles Sorted by Distance (Closest to Farthest):" << endl;
    printSortedObstacles(obstacleTree);
    cout << "Meters" << endl;

    return 0;
}