#include <iostream>
#include <vector>

using namespace std;

class OccupancyGrid2D {
private:
    int rows, cols;
    vector<vector<int>> grid;

public:
    OccupancyGrid2D(int r, int c) : rows(r), cols(c), grid(r, vector<int>(c, -1)) {} // Init all to -1 (Unmapped)

    void updateCell(int r, int c, int probability) {
        if (r >= 0 && r < rows && c >= 0 && c < cols) {
            grid[r][c] = probability;
        }
    }

    void renderMap() {
        cout << "SLAM Occupancy Grid Map (0 = Free, 100 = Wall, -1 = Unmapped):" << endl;
        for (int i = 0; i < rows; i++) {
            cout << "Row " << i << ": [ ";
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == -1) cout << "?  ";
                else if (grid[i][j] == 0) cout << ".  ";
                else cout << "X  "; // Wall
            }
            cout << "]" << endl;
        }
    }
};

int main() {
    cout << "--- DAY 10: SLAM OCCUPANCY GRID MAPPING ---" << endl << endl;

    OccupancyGrid2D map(4, 4);

    // Simulating radar mapping a room
    map.updateCell(0, 0, 0);   // Free space
    map.updateCell(0, 1, 0);   // Free space
    map.updateCell(0, 2, 100); // Detected Concrete Wall!
    map.updateCell(1, 0, 0);

    map.renderMap();

    return 0;
}