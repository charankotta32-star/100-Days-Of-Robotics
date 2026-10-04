#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>
#include <cstring>

using namespace std;

// CAN 2.0B Standard Message Frame
struct CANFrame {
    uint32_t id;      // Standard (11-bit) or Extended (29-bit) ID
    bool is_extended; // False = Standard, True = Extended
    uint8_t dlc;      // Data Length Code (0 to 8 bytes)
    uint8_t data[8];  // Payload
};

class CANAcceptanceFilter {
private:
    uint32_t filter_code;
    uint32_t filter_mask;

public:
    // Mask bit = 1 means "must match code bit exactly"; Mask bit = 0 means "don't care"
    CANAcceptanceFilter(uint32_t code, uint32_t mask)
        : filter_code(code), filter_mask(mask) {}

    [[nodiscard]] bool accept(uint32_t incoming_id) const {
        return (incoming_id & filter_mask) == (filter_code & filter_mask);
    }
};

class CANFramePacker {
public:
    // Packs two 16-bit motor PWM speeds into an 8-byte CAN frame
    static CANFrame packMotorSpeeds(uint32_t message_id, int16_t left_rpm, int16_t right_rpm) {
        CANFrame frame{};
        frame.id = message_id;
        frame.is_extended = false;
        frame.dlc = 4; // 2 bytes left + 2 bytes right = 4 bytes payload

        // Big-endian byte stuffing
        frame.data[0] = (left_rpm >> 8) & 0xFF;
        frame.data[1] = left_rpm & 0xFF;
        frame.data[2] = (right_rpm >> 8) & 0xFF;
        frame.data[3] = right_rpm & 0xFF;

        return frame;
    }

    // Unpacks motor speeds from payload
    static void unpackMotorSpeeds(const CANFrame& frame, int16_t& out_left, int16_t& out_right) {
        out_left  = (int16_t)((frame.data[0] << 8) | frame.data[1]);
        out_right = (int16_t)((frame.data[2] << 8) | frame.data[3]);
    }
};

int main() {
    cout << "--- DAY 43: CAN BUS 2.0B FRAME PACKER & ACCEPTANCE FILTER ---" << endl << endl;

    // Filter Config: Accept only Motor Controller messages in range 0x200 - 0x20F
    // Code = 0x200, Mask = 0x7F0 (Checks top 7 bits, ignores bottom 4 bits)
    CANAcceptanceFilter motorFilter(0x200, 0x7F0);

    // 1. Pack outbound Motor Command (Left: 300 RPM, Right: -250 RPM)
    CANFrame tx_frame = CANFramePacker::packMotorSpeeds(0x204, 300, -250);

    cout << "1. Outbound CAN Frame Transmitted:\n";
    cout << "   • ID       : 0x" << hex << uppercase << tx_frame.id << dec << "\n";
    cout << "   • DLC (Len): " << (int)tx_frame.dlc << " bytes\n";
    cout << "   • Payload  : [ ";
    for (int i = 0; i < tx_frame.dlc; ++i) {
        cout << "0x" << hex << uppercase << setw(2) << setfill('0') << (int)tx_frame.data[i] << " ";
    }
    cout << dec << setfill(' ') << "]\n\n";

    // 2. Acceptance Filter Evaluation on incoming bus traffic
    vector<uint32_t> incoming_ids = {
        0x204, // Motor command (Matches filter!)
        0x100, // IMU Telemetry (Should be ignored by motor MCU)
        0x20A, // Another motor driver command (Matches filter!)
        0x7DF  // Diagnostic broadcast (Should be ignored)
    };

    cout << "2. Hardware CAN Bus Acceptance Filter Pipeline (Mask: 0x7F0):\n";
    for (uint32_t id : incoming_ids) {
        bool accepted = motorFilter.accept(id);
        cout << "   • Message ID: 0x" << hex << uppercase << setw(3) << id << dec << " ➔ "
             << (accepted ? "✅ ACCEPTED (Passed to RX Queue)" : "⚪ REJECTED (Hardware Filtered)") << "\n";
    }

    // 3. Unpack received frame
    int16_t rx_left = 0, rx_right = 0;
    CANFramePacker::unpackMotorSpeeds(tx_frame, rx_left, rx_right);
    cout << "\n3. Unpacked Motor Data on Receiver Node:\n";
    cout << "   • Left Motor  : " << rx_left << " RPM\n";
    cout << "   • Right Motor : " << rx_right << " RPM\n";

    return 0;
}