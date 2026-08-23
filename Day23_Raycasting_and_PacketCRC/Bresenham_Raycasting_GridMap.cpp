#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int GRID_SIZE = 10;
const int CELL_UNKNOWN = -1;
const int CELL_FREE    = 0;
const int CELL_OCCUPIED = 100;

class OccupancyGrid2D {
private:
    int map[GRID_SIZE][GRID_SIZE];

public:
    OccupancyGrid2D() {
        for (int r = 0; r < GRID_SIZE; r++) {
            for (int c = 0; c < GRID_SIZE; c++) {
                map[r][c] = CELL_UNKNOWN;
            }
        }
    }

    // Bresenham's Line Algorithm for Fast Integer Raycasting
    void traceRay(int x0, int y0, int x1, int y1) {
        int dx = abs(x1 - x0);
        int dy = abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        int curr_x = x0;
        int curr_y = y0;

        while (true) {
            // Reached target obstacle cell
            if (curr_x == x1 && curr_y == y1) {
                map[curr_y][curr_x] = CELL_OCCUPIED; // Obstacle hit!
                break;
            }

            // Free space along the ray
            map[curr_y][curr_x] = CELL_FREE;

            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                curr_x += sx;
            }
            if (e2 < dx) {
                err += dx;
                curr_y += sy;
            }
        }
    }

    void display() const {
        cout << "   ";
        for (int c = 0; c < GRID_SIZE; c++) cout << c << " ";
        cout << "\n  +" << string(GRID_SIZE * 2, '-') << "\n";

        for (int r = GRID_SIZE - 1; r >= 0; r--) {
            cout << r << " | ";
            for (int c = 0; c < GRID_SIZE; c++) {
                if (map[r][c] == CELL_OCCUPIED) cout << "# "; // Obstacle
                else if (map[r][c] == CELL_FREE) cout << ". "; // Free space
                else cout << "? ";                            // Unknown
            }
            cout << "\n";
        }
    }
};

int main() {
    cout << "--- DAY 23: BRESENHAM OCCUPANCY GRID RAYCASTER (21MMH024F) ---" << endl << endl;

    OccupancyGrid2D slamGrid;
    int robot_x = 1, robot_y = 1;

    cout << "Robot Position: (" << robot_x << ", " << robot_y << ")\n";
    cout << "Tracing Range Sensor Pings to Obstacles at (8, 7) and (1, 8)...\n\n";

    // Ray 1: Sensor detects wall at (8, 7)
    slamGrid.traceRay(robot_x, robot_y, 8, 7);

    // Ray 2: Sensor detects pillar at (1, 8)
    slamGrid.traceRay(robot_x, robot_y, 1, 8);

    slamGrid.display();

    cout << "\nLegend: [#] Occupied Obstacle | [.] Cleared Free Space | [?] Unknown\n";
    return 0;
}