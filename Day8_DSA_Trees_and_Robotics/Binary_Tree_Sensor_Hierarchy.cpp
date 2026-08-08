#include <iostream>
#include <string>

using namespace std;

// Binary Tree Node representing a Subsystem
struct SystemNode {
    string system_name;
    SystemNode* left;  // Subsystem 1
    SystemNode* right; // Subsystem 2
};

int main() {
    cout << "--- DAY 8: ROBOT SYSTEM HIERARCHY TREE ---" << endl << endl;

    // Creating Root Node
    SystemNode* root = new SystemNode{"Master MCU (ESP32-S3)", nullptr, nullptr};

    // Creating Left and Right Children
    root->left = new SystemNode{"Vision System (mmWave Radar)", nullptr, nullptr};
    root->right = new SystemNode{"Drive System (PID Motors)", nullptr, nullptr};

    // Printing System Hierarchy
    cout << "Root System: " << root->system_name << endl;
    cout << " ├── Left Child:  " << root->left->system_name << endl;
    cout << " └── Right Child: " << root->right->system_name << endl;

    // Cleanup
    delete root->left;
    delete root->right;
    delete root;

    return 0;
}