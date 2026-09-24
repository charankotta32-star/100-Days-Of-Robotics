#include <iostream>
#include <vector>
#include <queue>
#include <string>

using namespace std;

class TaskDependencyDAG {
private:
    int num_tasks;
    vector<string> task_names;
    vector<vector<int>> adj_list;
    vector<int> in_degree;

public:
    TaskDependencyDAG(int count) : num_tasks(count) {
        task_names.resize(count);
        adj_list.resize(count);
        in_degree.resize(count, 0);
    }

    void setTaskName(int id, const string& name) {
        task_names[id] = name;
    }

    // u must complete before v can start (u -> v)
    void addDependency(int u, int v) {
        adj_list[u].push_back(v);
        in_degree[v]++;
    }

    void computeExecutionOrder() {
        queue<int> q;

        // 1. Push all tasks with zero incoming dependencies
        for (int i = 0; i < num_tasks; i++) {
            if (in_degree[i] == 0) {
                q.push(i);
            }
        }

        vector<int> order;

        // 2. Process tasks breadth-first
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            order.push_back(current);

            // Reduce in-degree for dependent tasks
            for (int neighbor : adj_list[current]) {
                in_degree[neighbor]--;
                if (in_degree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }

        // 3. Cycle Detection / Deadlock check
        if ((int)order.size() != num_tasks) {
            cout << "🚨 [DEADLOCK DETECTED] Circular dependency exists in task architecture!\n";
            return;
        }

        cout << "--- VALID AUTONOMOUS ROVER BOOTSTRAP PIPELINE ---\n";
        for (size_t i = 0; i < order.size(); i++) {
            cout << " Step " << (i + 1) << ": [" << order[i] << "] "
                 << task_names[order[i]];
            if (i < order.size() - 1) cout << "\n    ⬇\n";
        }
        cout << "\n-------------------------------------------------\n";
        cout << ">>> All " << num_tasks << " prerequisites resolved linearly in O(V + E) time.\n";
    }
};

int main() {
    cout << "--- DAY 39: KAHN'S TOPOLOGICAL SORT ALGORITHM (DSA) ---" << endl << endl;

    // 6 Robot initialization tasks
    TaskDependencyDAG pipeline(6);

    pipeline.setTaskName(0, "Read_Battery_Voltage_BMS");
    pipeline.setTaskName(1, "Calibrate_MPU6050_Gyro_Bias");
    pipeline.setTaskName(2, "Initialize_BTS7960_PWM_Timers");
    pipeline.setTaskName(3, "Mount_ESP32_WiFi_HTTP_Bridge");
    pipeline.setTaskName(4, "Engage_DWA_Motion_Planner");
    pipeline.setTaskName(5, "Launch_Autonomous_Navigation");

    // Establish hard prerequisites:
    // Battery check (0) must precede everything
    pipeline.addDependency(0, 1); // Check Battery -> Calibrate Gyro
    pipeline.addDependency(0, 2); // Check Battery -> Init PWM
    pipeline.addDependency(1, 4); // Calibrate Gyro -> Motion Planner
    pipeline.addDependency(2, 4); // Init PWM -> Motion Planner
    pipeline.addDependency(3, 5); // WiFi Bridge -> Launch Nav
    pipeline.addDependency(4, 5); // Motion Planner -> Launch Nav

    pipeline.computeExecutionOrder();

    return 0;
}