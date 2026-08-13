#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>

using namespace std;

// Calculates Pearson Correlation Coefficient r between Signal X and Signal Y
float calculatePearsonCorrelation(const vector<float>& x, const vector<float>& y) {
    int n = x.size();
    float sum_x = accumulate(x.begin(), x.end(), 0.0f);
    float sum_y = accumulate(y.begin(), y.end(), 0.0f);

    float mean_x = sum_x / n;
    float mean_y = sum_y / n;

    float numerator = 0.0, var_x = 0.0, var_y = 0.0;

    for (int i = 0; i < n; i++) {
        float diff_x = x[i] - mean_x;
        float diff_y = y[i] - mean_y;
        numerator += diff_x * diff_y;
        var_x += diff_x * diff_x;
        var_y += diff_y * diff_y;
    }

    return numerator / sqrt(var_x * var_y);
}

int main() {
    cout << "--- DAY 13: CROSS-REGION PEARSON CORRELATION ---" << endl << endl;

    // Forehead rPPG signal vs Left-Cheek rPPG signal
    vector<float> forehead_signal   = {0.1, 0.4, 0.8, 0.5, 0.2};
    vector<float> left_cheek_signal = {0.12, 0.39, 0.82, 0.48, 0.21}; // Strongly correlated!

    float r = calculatePearsonCorrelation(forehead_signal, left_cheek_signal);

    cout << "Forehead vs Left-Cheek Cross-Correlation Score: " << r << endl;
    if (r > 0.85) {
        cout << "[RESULT] HIGH SPATIAL COHERENCE CONFIRMED (Biological Signal)" << endl;
    } else {
        cout << "[RESULT] LOW COHERENCE (Inconsistent Signal / Synthetic)" << endl;
    }

    return 0;
}