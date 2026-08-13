#include <iostream>
#include <vector>

using namespace std;

struct FaceScore {
    int face_id;
    float authenticity_score;
};

// QuickSort Partition Step
int partition(vector<FaceScore>& arr, int low, int high) {
    float pivot = arr[high].authenticity_score;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        // Sort in DESCENDING order (highest score first)
        if (arr[j].authenticity_score > pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Recursive QuickSort
void quickSortScores(vector<FaceScore>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortScores(arr, low, pi - 1);
        quickSortScores(arr, pi + 1, high);
    }
}

int main() {
    cout << "--- DAY 13: QUICKSORT CONFIDENCE CLASSIFIER (DSA UNIT 1) ---" << endl << endl;

    vector<FaceScore> detected_faces = {
        {1, 23.5}, // Fake
        {2, 94.2}, // Real
        {3, 88.0}, // Real
        {4, 12.1}  // Fake
    };

    quickSortScores(detected_faces, 0, detected_faces.size() - 1);

    cout << "Sorted Multi-Face Authenticity Scores (Highest First):" << endl;
    for (const auto& face : detected_faces) {
        cout << "  Face #" << face.face_id << " -> Authenticity Score: " << face.authenticity_score << "%" << endl;
    }

    return 0;
}