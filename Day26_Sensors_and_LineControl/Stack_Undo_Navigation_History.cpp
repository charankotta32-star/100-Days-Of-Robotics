#include <iostream>
#include <string>

using namespace std;

struct Waypoint {
    string name;
    int x, y;
};

// Clean Stack ADT using dynamic array
class NavigationStack {
private:
    Waypoint* arr;
    int top;
    int capacity;

public:
    NavigationStack(int size = 10) : capacity(size), top(-1) {
        arr = new Waypoint[capacity];
    }

    bool isFull() { return top == capacity - 1; }
    bool isEmpty() { return top == -1; }

    void push(string name, int x, int y) {
        if (isFull()) {
            cout << "⚠️ Navigation History Stack Overflow!" << endl;
            return;
        }
        arr[++top] = {name, x, y};
        cout << "[NAV RECORD] Reached: " << name << " (" << x << ", " << y << ")" << endl;
    }

    Waypoint pop() {
        if (isEmpty()) {
            cout << "🚨 History Empty! At home base." << endl;
            return {"Home", 0, 0};
        }
        return arr[top--];
    }

    ~NavigationStack() {
        delete[] arr;
    }
};

int main() {
    cout << "--- DAY 26: STACK-BASED ROVER WAYPOINT BACKTRACKER (DSA) ---" << endl << endl;

    NavigationStack history(5);

    // Rover advances through waypoints
    history.push("Base_Dock", 0, 0);
    history.push("Corridor_A", 2, 0);
    history.push("Hazard_Junction", 2, 4);

    cout << "\n>>> [OBSTACLE ENCOUNTERED] Dead-end ahead! Retracing path:" << endl;

    while (!history.isEmpty()) {
        Waypoint prev = history.pop();
        cout << "  <- Backtracking to: " << prev.name << " (" << prev.x << ", " << prev.y << ")" << endl;
    }

    return 0;
}