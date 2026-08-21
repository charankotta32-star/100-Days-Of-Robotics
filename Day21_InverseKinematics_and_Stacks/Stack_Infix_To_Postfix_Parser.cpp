#include <iostream>
#include <stack>
#include <string>

using namespace std;

class ExpressionParser {
private:
    // Operator Precedence Evaluator
    int getPrecedence(char op) {
        if (op == '^') return 3;
        if (op == '*' || op == '/') return 2;
        if (op == '+' || op == '-') return 1;
        return -1;
    }

    bool isOperand(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
    }

public:
    // Converts Infix (Human-Readable) to Postfix (Stack-Executable)
    string infixToPostfix(string infix) {
        stack<char> op_stack;
        string postfix = "";

        for (int i = 0; i < infix.length(); i++) {
            char c = infix[i];

            // 1. If character is an operand (variable/number), append directly
            if (isOperand(c)) {
                postfix += c;
            }
            // 2. If '(', push to operator stack
            else if (c == '(') {
                op_stack.push('(');
            }
            // 3. If ')', pop and append all operators until '(' is encountered
            else if (c == ')') {
                while (!op_stack.empty() && op_stack.top() != '(') {
                    postfix += op_stack.top();
                    op_stack.pop();
                }
                if (!op_stack.empty()) op_stack.pop(); // Pop '('
            }
            // 4. If Operator
            else {
                while (!op_stack.empty() && getPrecedence(c) <= getPrecedence(op_stack.top())) {
                    postfix += op_stack.top();
                    op_stack.pop();
                }
                op_stack.push(c);
            }
        }

        // Pop remaining operators from stack
        while (!op_stack.empty()) {
            postfix += op_stack.top();
            op_stack.pop();
        }

        return postfix;
    }
};

int main() {
    cout << "--- DAY 21: STACK INFIX TO POSTFIX CONVERTER (DSA UNIT 2) ---" << endl << endl;

    ExpressionParser parser;

    // Example 1: Standard kinematics algebraic formula
    string expr1 = "(A+B)*(C-D)";
    cout << "1. Human Robot Command Infix:   " << expr1 << endl;
    cout << "   -> Executable Postfix Stack:  " << parser.infixToPostfix(expr1) << endl << endl;

    // Example 2: Complex threshold condition: X * Y + Z / W ^ P
    string expr2 = "X*Y+Z/W^P";
    cout << "2. Sensor Condition Infix:      " << expr2 << endl;
    cout << "   -> Executable Postfix Stack:  " << parser.infixToPostfix(expr2) << endl << endl;

    return 0;
}