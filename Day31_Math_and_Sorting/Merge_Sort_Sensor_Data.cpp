#include <iostream>
#include <vector>

using namespace std;

class MergeSorter {
private:
    // Merges two sorted halves back together
    void merge(vector<int>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        vector<int> L(n1), R(n2);
        for (int i = 0; i < n1; i++) L[i] = arr[left + i];
        for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) arr[k++] = L[i++];
            else arr[k++] = R[j++];
        }

        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];
    }

public:
    // Recursive Divide & Conquer Sort: O(N log N)
    void mergeSort(vector<int>& arr, int left, int right) {
        if (left >= right) return; // Base case: 1 element

        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);      // Sort left half
        mergeSort(arr, mid + 1, right); // Sort right half

        merge(arr, left, mid, right);   // Merge both halves
    }
};

int main() {
    cout << "--- DAY 31: DSA (MERGE SORT FOR LIDAR DATA) ---" << endl << endl;

    MergeSorter dsa;
    
    // Unsorted LiDAR distance pings (cm)
    vector<int> lidar_pings = {450, 12, 89, 34, 1, 99, 230, 42};

    cout << "Raw Unsorted Pings:   [ ";
    for (int p : lidar_pings) cout << p << " ";
    cout << "]" << endl;

    // Call Merge Sort
    dsa.mergeSort(lidar_pings, 0, lidar_pings.size() - 1);

    cout << "Merge Sorted Pings:   [ ";
    for (int p : lidar_pings) cout << p << " ";
    cout << "]" << endl;

    cout << "\n>>> Time Complexity: O(N log N) | Space Complexity: O(N)" << endl;

    return 0;
}