#include <iostream>
#include <stack>
#include <string>

using namespace std;

class BracketValidator {
private:
    bool isMatchingPair(char opening, char closing) {
        if (opening == '(' && closing == ')') return true;
        if (opening == '{' && closing == '}') return true;
        if (opening == '[' && closing == ']') return true;
        return false;
    }

public:
    bool isBalanced(string expression) {
        stack<char> s;

        for (int i = 0; i < expression.length(); i++) {
            char current = expression[i];

            // 1. If opening bracket, push to stack
            if (current == '(' || current == '{' || current == '[') {
                s.push(current);
            }
            // 2. If closing bracket
            else if (current == ')' || current == '}' || current == ']') {
                // If stack is empty, it means there is no opening bracket for this closing bracket
                if (s.empty()) {
                    return false;
                }

                // Pop the top element and check if it matches
                char top = s.top();
                s.pop();

                if (!isMatchingPair(top, current)) {
                    return false;
                }
            }
        }

        // 3. If stack is empty at the end, it's balanced
        return s.empty();
    }
};

int main() {
    cout << "--- DAY 35: DSA (STACK BALANCED PARENTHESES) ---" << endl << endl;

    BracketValidator validator;

    string test_cases[] = {
        "{[()]}",      // Balanced
        "{[(])}",      // Unbalanced (Mismatched)
        "((()))",      // Balanced
        "[{}]()",      // Balanced
        "[{"           // Unbalanced (Missing closing)
    };

    cout << "Expression   | Status" << endl;
    cout << "---------------------------------" << endl;

    for (string expr : test_cases) {
        cout << left << setw(12) << expr << " | ";
        if (validator.isBalanced(expr)) {
            cout << "✅ Balanced" << endl;
        } else {
            cout << "❌ Unbalanced" << endl;
        }
    }

    return 0;
}