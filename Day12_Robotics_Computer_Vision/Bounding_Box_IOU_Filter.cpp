#include <iostream>
#include <algorithm>

using namespace std;

struct BoundingBox {
    float x1, y1, x2, y2; // Top-left (x1, y1) and Bottom-right (x2, y2)
};

// Calculates IoU (Intersection Over Union) between two bounding boxes
float calculateIoU(BoundingBox b1, BoundingBox b2) {
    // 1. Calculate intersection box coordinates
    float inter_x1 = max(b1.x1, b2.x1);
    float inter_y1 = max(b1.y1, b2.y1);
    float inter_x2 = min(b1.x2, b2.x2);
    float inter_y2 = min(b1.y2, b2.y2);

    float inter_width  = max(0.0f, inter_x2 - inter_x1);
    float inter_height = max(0.0f, inter_y2 - inter_y1);
    float inter_area   = inter_width * inter_height;

    // 2. Calculate individual box areas
    float b1_area = (b1.x2 - b1.x1) * (b1.y2 - b1.y1);
    float b2_area = (b2.x2 - b2.x1) * (b2.y2 - b2.y1);

    // 3. IoU = Intersection / Union
    float union_area = b1_area + b2_area - inter_area;
    return inter_area / union_area;
}

int main() {
    cout << "--- DAY 12: BOUNDING BOX INTERSECTION OVER UNION (IoU) ---" << endl << endl;

    BoundingBox ground_truth = {10.0, 10.0, 50.0, 50.0}; // Ground Truth Object Box
    BoundingBox prediction   = {12.0, 12.0, 52.0, 52.0}; // AI Model Prediction Box

    float iou = calculateIoU(ground_truth, prediction);

    cout << "Ground Truth Box: (10, 10) to (50, 50)" << endl;
    cout << "AI Prediction Box: (12, 12) to (52, 52)" << endl;
    cout << "Calculated IoU Score: " << iou * 100.0f << "% Overlap Match!" << endl;

    return 0;
}