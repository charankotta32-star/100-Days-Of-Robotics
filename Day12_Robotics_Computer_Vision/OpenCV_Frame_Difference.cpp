#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class FrameDifferencer {
private:
    int motion_threshold = 30; // Minimum pixel delta to register motion

public:
    void detectMotion(const vector<vector<int>>& frame1, const vector<vector<int>>& frame2) {
        int rows = frame1.size();
        int cols = frame1[0].size();
        int changed_pixels = 0;

        cout << "Motion Difference Matrix (1 = Movement, 0 = Static):" << endl;
        for (int i = 0; i < rows; i++) {
            cout << "  [ ";
            for (int j = 0; j < cols; j++) {
                int delta = abs(frame2[i][j] - frame1[i][j]);
                if (delta > motion_threshold) {
                    cout << "1  ";
                    changed_pixels++;
                } else {
                    cout << "0  ";
                }
            }
            cout << "]" << endl;
        }

        cout << "[VISION ENGINE] Total Motion Pixels: " << changed_pixels << endl;
    }
};

int main() {
    cout << "--- DAY 12: COMPUTER VISION FRAME DIFFERENCING ---" << endl << endl;

    // Simulated 3x3 camera frame at t=0s and t=0.1s
    vector<vector<int>> prev_frame = {
        {100, 100, 100},
        {100, 100, 100},
        {100, 100, 100}
    };

    vector<vector<int>> curr_frame = {
        {100, 100, 100},
        {100, 240, 100}, // Object moved into center pixel!
        {100, 100, 100}
    };

    FrameDifferencer visionNode;
    visionNode.detectMotion(prev_frame, curr_frame);

    return 0;
}