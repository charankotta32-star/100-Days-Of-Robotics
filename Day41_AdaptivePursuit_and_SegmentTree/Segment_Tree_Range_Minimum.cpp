#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

class SegmentTreeRMQ {
private:
    int n;
    vector<double> tree;
    const double INF = 1e9;

    void build(const vector<double>& arr, int node, int start, int end) {
        if (start == end) {
            tree[node] = arr[start];
            return;
        }
        int mid = start + (end - start) / 2;
        int left_child = 2 * node + 1;
        int right_child = 2 * node + 2;

        build(arr, left_child, start, mid);
        build(arr, right_child, mid + 1, end);

        tree[node] = min(tree[left_child], tree[right_child]);
    }

    double queryMin(int node, int start, int end, int l, int r) const {
        // Range completely outside query interval [l, r]
        if (r < start || end < l) return INF;

        // Range completely inside query interval [l, r]
        if (l <= start && end <= r) return tree[node];

        // Partial overlap: search both sub-branches
        int mid = start + (end - start) / 2;
        double left_min = queryMin(2 * node + 1, start, mid, l, r);
        double right_min = queryMin(2 * node + 2, mid + 1, end, l, r);

        return min(left_min, right_min);
    }

    void updatePoint(int node, int start, int end, int idx, double val) {
        if (start == end) {
            tree[node] = val;
            return;
        }
        int mid = start + (end - start) / 2;
        int left_child = 2 * node + 1;
        int right_child = 2 * node + 2;

        if (idx <= mid) updatePoint(left_child, start, mid, idx, val);
        else updatePoint(right_child, mid + 1, end, idx, val);

        tree[node] = min(tree[left_child], tree[right_child]);
    }

public:
    explicit SegmentTreeRMQ(const vector<double>& arr) {
        n = arr.size();
        tree.assign(4 * n, INF);
        if (n > 0) build(arr, 0, 0, n - 1);
    }

    [[nodiscard]] double query(int l, int r) const {
        return queryMin(0, 0, n - 1, l, r);
    }

    void update(int idx, double val) {
        updatePoint(0, 0, n - 1, idx, val);
    }
};

int main() {
    cout << "--- DAY 41: SEGMENT TREE RANGE MINIMUM QUERY (DSA UNIT 5) ---" << endl << endl;

    // 8-Sector LiDAR distance readings (meters) covering 0° to 360° around rover
    // Sectors: [0:Front, 1:Front-R, 2:Right, 3:Rear-R, 4:Rear, 5:Rear-L, 6:Left, 7:Front-L]
    vector<double> lidar_sectors = {3.5, 4.2, 1.8, 5.0, 6.2, 2.4, 0.85, 2.1};

    SegmentTreeRMQ rmq(lidar_sectors);
    cout << fixed << setprecision(2);

    cout << "LiDAR Sector Readings: [ ";
    for (double d : lidar_sectors) cout << d << "m ";
    cout << "]\n\n";

    // Query 1: Front cone clearance (Sectors 0 to 1, and 7)
    cout << "Querying Front-Right Sector [0 to 2]:\n";
    cout << " -> Closest Obstacle Distance: " << rmq.query(0, 2) << " m (O(log N))\n\n";

    cout << "Querying Left Flank Sector [5 to 7]:\n";
    cout << " -> Closest Obstacle Distance: " << rmq.query(5, 7) << " m\n\n";

    // Point update: Dynamic obstacle suddenly enters Sector 0 (Front) at 0.45m!
    cout << "⚡ [UPDATE] Dynamic obstacle steps into Sector 0! Range drops to 0.45m.\n";
    rmq.update(0, 0.45);

    cout << "Re-querying Front-Right Sector [0 to 2] after update:\n";
    cout << " -> 🚨 IMMEDIATE THREAT DETECTED at: " << rmq.query(0, 2) << " m!\n";

    return 0;
}