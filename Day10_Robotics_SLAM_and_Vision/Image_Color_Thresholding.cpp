#include <iostream>
#include <vector>

using namespace std;

class VisionProcessor {
private:
    int threshold;

public:
    VisionProcessor(int thresh) : threshold(thresh) {}

    void applyBinaryThreshold(const vector<vector<int>>& image_matrix) {
        int rows = image_matrix.size();
        int cols = image_matrix[0].size();

        cout << "Binary Thresholded Image Matrix (Threshold = " << threshold << "):" << endl;
        for (int i = 0; i < rows; i++) {
            cout << "  [ ";
            for (int j = 0; j < cols; j++) {
                // Pixel > Threshold becomes 255 (White), else 0 (Black)
                int binary_pixel = (image_matrix[i][j] >= threshold) ? 255 : 0;
                cout << binary_pixel << "\t";
            }
            cout << "]" << endl;
        }
    }
};

int main() {
    cout << "--- DAY 10: COMPUTER VISION MATRIX THRESHOLDING ---" << endl << endl;

    // Simulated 3x3 grayscale camera frame (0 = pitch black, 255 = pure white)
    vector<vector<int>> camera_frame = {
        {45,  120, 210},
        {10,  200, 250},
        {180, 30,  85}
    };

    VisionProcessor cvEngine(128); // Threshold = 128
    cvEngine.applyBinaryThreshold(camera_frame);

    return 0;
}