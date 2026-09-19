#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdint>

using namespace std;

class FastCRC16 {
private:
    uint16_t crc_table[256];
    const uint16_t POLYNOMIAL = 0x1021; // CRC-16 CCITT Standard

    // Precomputes the 256-entry table for O(1) byte lookups
    void generateTable() {
        for (int i = 0; i < 256; i++) {
            uint16_t curr = i << 8;
            for (int j = 0; j < 8; j++) {
                if (curr & 0x8000) {
                    curr = (curr << 1) ^ POLYNOMIAL;
                } else {
                    curr <<= 1;
                }
            }
            crc_table[i] = curr;
        }
    }

public:
    FastCRC16() {
        generateTable();
    }

    // High-speed O(N) calculation: 1 table lookup per byte
    uint16_t calculate(const vector<uint8_t>& data) {
        uint16_t crc = 0xFFFF; // Initial pre-set

        for (uint8_t byte : data) {
            uint8_t lut_idx = (crc >> 8) ^ byte;
            crc = (crc << 8) ^ crc_table[lut_idx];
        }
        return crc;
    }

    bool verify(const vector<uint8_t>& payload, uint16_t expected_crc) {
        return calculate(payload) == expected_crc;
    }
};

int main() {
    cout << "--- DAY 37: INDUSTRIAL TABLE-DRIVEN CRC-16 CCITT ENGINE ---" << endl << endl;

    FastCRC16 crcEngine;

    // Outbound ESP32 Motor Command Packet: [0xAA, 0x55, CMD: 0x01 (DRIVE), V: 120, W: 45]
    vector<uint8_t> packet = {0xAA, 0x55, 0x01, 120, 45};

    uint16_t checksum = crcEngine.calculate(packet);

    cout << "1. Transmitter Output:\n";
    cout << "   Payload Stream : [ 0xAA, 0x55, 0x01, 120, 45 ]\n";
    cout << "   Computed CRC-16: 0x" << hex << uppercase << checksum << dec << "\n\n";

    // 2. Receiver Test 1: Clean transmission
    cout << "2. Receiver Audit (Clean Frame):\n";
    if (crcEngine.verify(packet, checksum)) {
        cout << "   ✅ [FRAME INTEGRITY CONFIRMED] Packet approved for motor actuation.\n\n";
    } else {
        cout << "   ❌ Corrupted.\n\n";
    }

    // 3. Receiver Test 2: 1-bit electrical motor noise corruption
    cout << "3. Receiver Audit (Corrupted by Motor Inductive Spike):\n";
    vector<uint8_t> corrupted_packet = packet;
    corrupted_packet[3] = 121; // Velocity corrupted 120 -> 121

    if (crcEngine.verify(corrupted_packet, checksum)) {
        cout << "   ✅ Valid.\n";
    } else {
        cout << "   🚨 [CRC-16 REJECTION] Bit flip detected! Corrupted packet purged from buffer.\n";
    }

    return 0;
}