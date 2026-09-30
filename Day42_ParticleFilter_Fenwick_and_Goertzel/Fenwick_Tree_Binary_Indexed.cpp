#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

class FenwickTree {
private:
    int size;
    vector<long long> bit; // 1-based indexing tree

public:
    explicit FenwickTree(int n) : size(n), bit(n + 1, 0) {}

    // Adds 'delta' to element at 1-based index 'idx': O(log N)
    void update(int idx, long long delta) {
        while (idx <= size) {
            bit[idx] += delta;
            idx += (idx & -idx); // Extract lowest set bit and advance
        }
    }

    // Computes prefix sum from index 1 to 'idx': O(log N)
    [[nodiscard]] long long queryPrefix(int idx) const {
        long long sum = 0;
        while (idx > 0) {
            sum += bit[idx];
            idx -= (idx & -idx); // Clear lowest set bit and cascade up
        }
        return sum;
    }

    // Computes range sum [L, R] in O(log N)
    [[nodiscard]] long long queryRange(int l, int r) const {
        if (l > r || l <= 0) return 0;
        return queryPrefix(r) - queryPrefix(l - 1);
    }
};

int main() {
    cout << "--- DAY 42: BINARY INDEXED TREE (FENWICK TREE) ---" << endl << endl;

    // Simulated 8-slot motor power consumption log (milliampere-seconds / mAs)
    vector<int> motor_current_log = {150, 220, 180, 450 /* spike */, 210, 190, 310, 160};
    int n = motor_current_log.size();

    FenwickTree fenwick(n);

    // Build tree: O(N log N)
    for (int i = 0; i < n; i++) {
        fenwick.update(i + 1, motor_current_log[i]);
    }

    cout << "Raw Motor Current Log: [ ";
    for (int ma : motor_current_log) cout << ma << "mA ";
    cout << "]\n\n";

    // Query 1: Total power used in first 4 time slots [1 to 4]
    cout << "Prefix Energy Query [1 to 4]: " << fenwick.queryPrefix(4) << " mAs\n";

    // Query 2: Range Energy Query over intermediate run [3 to 6]
    cout << "Range Energy Query [3 to 6]:  " << fenwick.queryRange(3, 6) << " mAs\n\n";

    // Dynamic Point Update: Time slot 3 updates with additional current draw (+100mA)
    cout << "⚡ [UPDATE] Slot #3 draws extra +100mA spike during skid-steer pivot.\n";
    fenwick.update(3, 100);

    cout << "Range Energy Query [3 to 6] after update: " << fenwick.queryRange(3, 6) << " mAs (O(log N))\n";

    return 0;
}