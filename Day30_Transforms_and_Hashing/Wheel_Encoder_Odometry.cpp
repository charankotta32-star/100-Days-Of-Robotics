#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

class EncoderMath {
private:
    double wheel_radius_m;
    int ticks_per_revolution;
    double meters_per_tick;

public:
    EncoderMath(double radius_meters, int ppr)
        : wheel_radius_m(radius_meters), ticks_per_revolution(ppr) {

        // Circumference = 2 * pi * r
        double circumference = 2.0 * M_PI * wheel_radius_m;

        // How much physical distance 1 single tick represents
        meters_per_tick = circumference / (double)ticks_per_revolution;
    }

    double getDistanceMeters(long cumulative_ticks) {
        return (double)cumulative_ticks * meters_per_tick;
    }
};

int main() {
    cout << "--- DAY 30: WHEEL ENCODER TICKS TO DISTANCE MATH ---" << endl << endl;

    // Rover Specs: 110mm wheels (0.055m radius), Motor Encoder = 330 Pulses Per Rev
    EncoderMath left_wheel(0.055, 330);
    cout << fixed << setprecision(3);

    long simulated_encoder_ticks[] = {0, 165, 330, 990, -330};

    cout << "Wheel Radius: 55mm | Encoder Resolution: 330 PPR" << endl;
    cout << "------------------------------------------------" << endl;

    for (int i = 0; i < 5; i++) {
        long ticks = simulated_encoder_ticks[i];
        double distance = left_wheel.getDistanceMeters(ticks);

        cout << "Encoder Read: " << setw(5) << ticks << " ticks  --->  Physical Distance: "
             << setw(6) << distance << " meters" << endl;
    }

    return 0;
}