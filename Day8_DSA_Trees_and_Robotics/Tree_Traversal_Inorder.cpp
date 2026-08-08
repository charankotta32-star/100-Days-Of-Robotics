#include <iostream>
#include <string>

using namespace std;

struct DiagnosticNode {
    int error_code;
    string component;
    DiagnosticNode* left;
    DiagnosticNode* right;
};

// Recursive Inorder Traversal: Left -> Root -> Right
void runDiagnosticsInorder(DiagnosticNode* root) {
    if (root == nullptr) return;

    runDiagnosticsInorder(root->left);
    cout << "[CHECKING CODE " << root->error_code << "] -> Component: " << root->component << endl;
    runDiagnosticsInorder(root->right);
}

int main() {
    cout << "--- DAY 8: TREE INORDER DIAGNOSTIC TRAVERSAL ---" << endl << endl;

    DiagnosticNode* diagTree = new DiagnosticNode{200, "Camera JTAG", nullptr, nullptr};
    diagTree->left = new DiagnosticNode{100, "Power Rail 5V", nullptr, nullptr};
    diagTree->right = new DiagnosticNode{300, "LoRa SPI Bus", nullptr, nullptr};

    cout << "Executing Diagnostic Sequence (Inorder Traversal):" << endl;
    runDiagnosticsInorder(diagTree);

    // Cleanup
    delete diagTree->left;
    delete diagTree->right;
    delete diagTree;

    return 0;
}