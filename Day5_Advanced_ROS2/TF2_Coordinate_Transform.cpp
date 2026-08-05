#include <iostream>
#include <string>

using namespace std;

// Simulating a ROS 2 geometry_msgs::msg::Transform
struct Transform3D {
    string child_frame;
    float x_offset, y_offset, z_offset;
};

class TFBroadcaster {
public:
    void broadcastTransform(string parent, Transform3D tf) {
        cout << "[TF2 BROADCASTER] Mapping frame '" << tf.child_frame
             << "' to '" << parent << "'" << endl;
        cout << " -> Translation [X: " << tf.x_offset
             << "m, Y: " << tf.y_offset << "m, Z: " << tf.z_offset << "m]" << endl << endl;
    }
};

int main() {
    cout << "--- DAY 5: ROS 2 COORDINATE TRANSFORMS (TF2) ---" << endl << endl;

    TFBroadcaster tf_tree;

    // The radar is mounted 10cm forward (X) and 5cm high (Z) from the robot center
    Transform3D radar_tf = {"radar_link", 0.10, 0.0, 0.05};

    // The camera is mounted 5cm back (-X) and 20cm high (Z)
    Transform3D camera_tf = {"camera_link", -0.05, 0.0, 0.20};

    tf_tree.broadcastTransform("base_link", radar_tf);
    tf_tree.broadcastTransform("base_link", camera_tf);

    return 0;
}