#include <iostream>
#include <stack>
#include <string>
#include <cctype>

using namespace std;

// Expression Tree Node
struct TreeNode {
    string value;
    TreeNode* left;
    TreeNode* right;

    TreeNode(string val) : value(val), left(nullptr), right(nullptr) {}
};

class ExpressionTree {
private:
    TreeNode* root;

    bool isOperator(const string& s) {
        return (s == "+" || s == "-" || s == "*" || s == "/" || s == "^");
    }

    // Recursive Tree Evaluation
    double evaluateRecursive(TreeNode* node) {
        if (!node) return 0.0;

        // Leaf Node (Operand / Number)
        if (!isOperator(node->value)) {
            return stod(node->value);
        }

        // Evaluate Left and Right Subtrees
        double left_val  = evaluateRecursive(node->left);
        double right_val = evaluateRecursive(node->right);

        if (node->value == "+") return left_val + right_val;
        if (node->value == "-") return left_val - right_val;
        if (node->value == "*") return left_val * right_val;
        if (node->value == "/") {
            if (right_val == 0.0) {
                cout << "🚨 [ERROR] Division by zero in spatial calculation!" << endl;
                return 0.0;
            }
            return left_val / right_val;
        }
        return 0.0;
    }

    // Recursive In-Order Traversal (Reconstructs Mathematical Equation)
    void inorderPrint(TreeNode* node) {
        if (node) {
            if (isOperator(node->value)) cout << "(";
            inorderPrint(node->left);
            cout << " " << node->value << " ";
            inorderPrint(node->right);
            if (isOperator(node->value)) cout << ")";
        }
    }

    void destroyTree(TreeNode* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    ExpressionTree() : root(nullptr) {}

    // Constructs Binary Expression Tree from Postfix Tokens
    void buildFromPostfix(const vector<string>& tokens) {
        stack<TreeNode*> st;

        for (const string& token : tokens) {
            if (!isOperator(token)) {
                st.push(new TreeNode(token));
            } else {
                TreeNode* node = new TreeNode(token);
                // Pop right child first, then left child
                if (!st.empty()) { node->right = st.top(); st.pop(); }
                if (!st.empty()) { node->left  = st.top(); st.pop(); }
                st.push(node);
            }
        }

        if (!st.empty()) {
            root = st.top();
            st.pop();
        }
    }

    double evaluate() {
        return evaluateRecursive(root);
    }

    void displayEquation() {
        cout << "Reconstructed Expression Tree Equation: ";
        inorderPrint(root);
        cout << endl;
    }

    ~ExpressionTree() {
        destroyTree(root);
    }
};

int main() {
    cout << "--- DAY 24: BINARY EXPRESSION TREE EVALUATOR (DSA UNIT 3) ---" << endl << endl;

    ExpressionTree exprTree;

    // Postfix tokens representing: (15 + 5) * (8 - 2)
    // Postfix = 15 5 + 8 2 - *
    vector<string> postfix_tokens = {"15", "5", "+", "8", "2", "-", "*"};

    cout << "Ingesting Postfix Tokens: [ 15, 5, +, 8, 2, -, * ]" << endl;
    exprTree.buildFromPostfix(postfix_tokens);
    exprTree.displayEquation();

    double result = exprTree.evaluate();
    cout << ">>> Evaluated Numerical Value: " << result << endl << endl;

    return 0;
}