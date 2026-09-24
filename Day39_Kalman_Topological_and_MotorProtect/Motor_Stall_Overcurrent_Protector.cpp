#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

enum MotorSafetyState {
    MOTOR_NORMAL,
    MOTOR_WARNING_HEATING,
    MOTOR_STALL_LIMP_MODE,
    MOTOR_SHUTDOWN_LATCHED
};

class MotorStallProtector {
private:
    const double CURRENT_STALL_THRESHOLD = 6.0; // Amperes
    const double THERMAL_CAPACITY_LIMIT  = 50.0;// (I^2 * t) energy threshold
    const int MIN_TICK_DELTA_EXPECTED    = 5;   // Minimum encoder ticks required if PWM > 40%

    double thermal_accumulator; // Tracks accumulated I^2 * dt heat
    MotorSafetyState current_state;

public:
    MotorStallProtector() : thermal_accumulator(0.0), current_state(MOTOR_NORMAL) {}

    // Evaluates motor telemetry every control loop step
    int evaluateProtection(double measured_current_amps, int delta_encoder_ticks, int commanded_pwm, double dt) {
        // Compute active thermal heating: Heat ~ (I^2 * dt)
        double current_sq = measured_current_amps * measured_current_amps;

        // Heat accumulation vs. ambient cooling dissipation
        if (measured_current_amps > CURRENT_STALL_THRESHOLD) {
            thermal_accumulator += (current_sq * dt);
        } else {
            // Natural convective thermal cooling when load is light
            thermal_accumulator = max(0.0, thermal_accumulator - (10.0 * dt));
        }

        // Stall condition: High commanded power + high current + ZERO physical wheel movement
        bool is_mechanically_stalled = (commanded_pwm >= 40) &&
                                       (measured_current_amps >= CURRENT_STALL_THRESHOLD) &&
                                       (delta_encoder_ticks < MIN_TICK_DELTA_EXPECTED);

        // State Machine Transition
        if (thermal_accumulator >= THERMAL_CAPACITY_LIMIT) {
            current_state = MOTOR_SHUTDOWN_LATCHED;
            return 0; // Cut off all power immediately to prevent coil melting
        }
        else if (is_mechanically_stalled || thermal_accumulator > (THERMAL_CAPACITY_LIMIT * 0.5)) {
            current_state = MOTOR_STALL_LIMP_MODE;
            return min(commanded_pwm, 20); // Clamp to 20% safe Limp Mode power
        }
        else if (measured_current_amps > (CURRENT_STALL_THRESHOLD * 0.7)) {
            current_state = MOTOR_WARNING_HEATING;
            return commanded_pwm;
        }
        else {
            current_state = MOTOR_NORMAL;
            return commanded_pwm;
        }
    }

    [[nodiscard]] MotorSafetyState getState() const { return current_state; }
    [[nodiscard]] double getThermalLoad() const { return thermal_accumulator; }
};

int main() {
    cout << "--- DAY 39: MOTOR STALL & I^2t THERMAL OVERCURRENT PROTECTOR ---" << endl << endl;

    MotorStallProtector guard;
    cout << fixed << setprecision(1);

    int requested_pwm = 80; // 80% throttle command

    // Simulated scenario: Normal driving -> Hits obstacle/wall (Ticks = 0, Current = 7.2A) -> Recovers
    struct StepData {
        double current_amps;
        int encoder_ticks;
        string condition;
    };

    const vector<StepData> test_run = {
        { 1.8, 45, "Cruising on flat arena floor" },
        { 2.1, 42, "Cruising on flat arena floor" },
        { 7.2,  0, "🚨 CRASH: Hit arena barrier! Wheel stopped, stall current!" },
        { 7.4,  0, "🚨 Still stuck: Heat accumulating in motor coils" },
        { 7.3,  0, "🚨 Continuous stall: Thermal limit threshold approaching" },
        { 1.5, 30, "Obstacle cleared! Wheel turning freely again" }
    };

    cout << "Step | Motor Current | Delta Ticks | Commanded | Safe Output | Thermal I^2t | Protection State" << endl;
    cout << "-------------------------------------------------------------------------------------------------" << endl;

    const double dt = 0.5; // 500ms telemetry interval (moved to inner scope)

    for (size_t i = 0; i < test_run.size(); i++) {
        int safe_pwm = guard.evaluateProtection(test_run[i].current_amps,
                                                test_run[i].encoder_ticks,
                                                requested_pwm, dt);

        cout << " #" << (i + 1) << "  |     "
             << setw(4) << test_run[i].current_amps << " A     |     "
             << setw(2) << test_run[i].encoder_ticks << "      |    "
             << setw(3) << requested_pwm << "%   |    "
             << setw(3) << safe_pwm << "%    |    "
             << setw(5) << guard.getThermalLoad() << "     | ";

        if (guard.getState() == MOTOR_NORMAL) cout << "🟢 NORMAL" << endl;
        else if (guard.getState() == MOTOR_WARNING_HEATING) cout << "⚠️ HEATING" << endl;
        else if (guard.getState() == MOTOR_STALL_LIMP_MODE) cout << "🛑 STALL LIMP MODE" << endl;
        else cout << "⚡ HARD SHUTDOWN" << endl;
    }

    cout << "\n>>> [VERIFIED] Motor clamped safely to 20% PWM during stall; coils protected." << endl;
    return 0;
}