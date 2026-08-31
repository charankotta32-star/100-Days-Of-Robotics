#include <iostream>
#include <iomanip>
#include <cstdint>

using namespace std;

class COATimerMath {
public:
    // Calculates the required timer limit (Auto-Reload Register) for a specific frequency
    uint32_t calculateTimerTicks(uint32_t cpu_clock_hz, uint32_t target_pwm_hz) {
        if (target_pwm_hz == 0) return 0;
        return cpu_clock_hz / target_pwm_hz;
    }

    // Calculates the specific compare value for a target Duty Cycle %
    uint32_t calculateDutyCycleTicks(uint32_t total_ticks, double duty_cycle_percent) {
        if (duty_cycle_percent < 0.0 || duty_cycle_percent > 100.0) return 0;
        return (uint32_t)((duty_cycle_percent / 100.0) * total_ticks);
    }
};

int main() {
    cout << "--- DAY 31: COA (CPU CLOCK CYCLES & PWM MATH) ---" << endl << endl;

    COATimerMath coa;

    uint32_t ESP32_CLOCK = 80000000; // 80 MHz System Clock
    uint32_t MOTOR_PWM_FREQ = 1000;  // 1 kHz target frequency for TB6612FNG Driver

    uint32_t total_timer_ticks = coa.calculateTimerTicks(ESP32_CLOCK, MOTOR_PWM_FREQ);
    
    // Command motor to run at 75% speed
    double target_duty_cycle = 75.0; 
    uint32_t duty_ticks = coa.calculateDutyCycleTicks(total_timer_ticks, target_duty_cycle);

    cout << "CPU Clock Frequency: " << ESP32_CLOCK / 1000000 << " MHz" << endl;
    cout << "Target PWM Frequency: " << MOTOR_PWM_FREQ << " Hz" << endl;
    cout << "------------------------------------------------" << endl;
    cout << "=> Timer Maximum Limit (Period): " << total_timer_ticks << " clock ticks" << endl;
    cout << "=> Ticks required for " << target_duty_cycle << "% Duty Cycle: " << duty_ticks << " ticks" << endl;

    return 0;
}