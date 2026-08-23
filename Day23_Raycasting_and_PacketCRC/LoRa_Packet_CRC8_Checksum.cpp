#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdint>

using namespace std;


class CRC8Engine {
private:
    const uint8_t POLYNOMIAL = 0x07; // Standard Dallas/Maxim CRC-8 Polynomial (x^8 + x^2 + x + 1)

public:
    uint8_t calculateCRC(const vector<uint8_t>& data) {
        uint8_t crc = 0x00; // Initial value

        for (uint8_t byte : data) {
            crc ^= byte;
            for (int bit = 0; bit < 8; bit++) {
                if (crc & 0x80) {
                    crc = (crc << 1) ^ POLYNOMIAL;
                } else {
                    crc <<= 1;
                }
            }
        }
        return crc;
    }

    bool verifyPacket(const vector<uint8_t>& packet_with_crc) {
        if (packet_with_crc.empty()) return false;

        // Extract payload and received checksum
        vector<uint8_t> payload(packet_with_crc.begin(), packet_with_crc.end() - 1);
        uint8_t received_crc = packet_with_crc.back();
        uint8_t computed_crc = calculateCRC(payload);

        return (received_crc == computed_crc);
    }
};

int main() {
    cout << "--- DAY 23: LORA TELEMETRY CRC-8 CHECKSUM VERIFIER ---" << endl << endl;

    CRC8Engine crc;

    // ResQ-Sanjeevini LoRa Packet: [NodeID: 0x01, Vitals: 0x48 (72 BPM), Breathing: 0x10 (16 BrPM), Battery: 0x64 (100%)]
    vector<uint8_t> lora_payload = {0x01, 0x48, 0x10, 0x64};

    uint8_t checksum = crc.calculateCRC(lora_payload);
    cout << "1. Transmitter Side:" << endl;
    cout << "   Payload: [0x01, 0x48, 0x10, 0x64] -> Computed CRC-8: 0x"
         << hex << uppercase << (int)checksum << dec << endl;

    // Packet assembled with checksum at tail
    vector<uint8_t> tx_packet = lora_payload;
    tx_packet.push_back(checksum);

    // 2. Receiver Side (Uncorrupted Packet)
    cout << "\n2. Receiver Side (Healthy Transmission):" << endl;
    if (crc.verifyPacket(tx_packet)) {
        cout << "   ✅ [PACKET VALID] Checksum matched. Forwarding vitals to Incident Command." << endl;
    } else {
        cout << "   ❌ [PACKET CORRUPTED] Discarding packet." << endl;
    }

    // 3. Receiver Side (Simulated Bit-Flip Noise through Debris)
    cout << "\n3. Receiver Side (Corrupted Bit via Concrete Interference):" << endl;
    vector<uint8_t> noisy_packet = tx_packet;
    noisy_packet[1] = 0xFF; // Heart rate corrupted by RF noise spike

    if (crc.verifyPacket(noisy_packet)) {
        cout << "   ✅ [PACKET VALID]" << endl;
    } else {
        cout << "   🚨 [CRC MISMATCH DETECTED] Packet corrupted in transit. Discarded safely." << endl;
    }

    return 0;
}