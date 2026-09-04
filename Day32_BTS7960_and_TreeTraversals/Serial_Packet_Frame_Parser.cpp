#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>

using namespace std;

enum RxState {
    STATE_WAIT_HEADER1,
    STATE_WAIT_HEADER2,
    STATE_READ_LENGTH,
    STATE_COLLECT_PAYLOAD,
    STATE_VERIFY_CHECKSUM
};

class UARTFrameParser {
private:
    const uint8_t HEADER_1 = 0xAA;
    const uint8_t HEADER_2 = 0x55;

    RxState state;
    uint8_t payload_length;
    vector<uint8_t> payload_buffer;
    uint8_t computed_checksum;

public:
    UARTFrameParser() : state(STATE_WAIT_HEADER1), payload_length(0), computed_checksum(0) {}

    // Ingests bytes one-by-one as they arrive from UART hardware buffer
    void parseByte(uint8_t byte) {
        switch (state) {
            case STATE_WAIT_HEADER1:
                if (byte == HEADER_1) {
                    state = STATE_WAIT_HEADER2;
                }
                break;

            case STATE_WAIT_HEADER2:
                if (byte == HEADER_2) {
                    state = STATE_READ_LENGTH;
                } else {
                    state = STATE_WAIT_HEADER1;
                }
                break;

            case STATE_READ_LENGTH:
                payload_length = byte;
                payload_buffer.clear();
                computed_checksum = byte; // Initialize checksum with length byte
                state = STATE_COLLECT_PAYLOAD;
                break;

            case STATE_COLLECT_PAYLOAD:
                payload_buffer.push_back(byte);
                computed_checksum ^= byte; // XOR checksum calculation

                if (payload_buffer.size() == payload_length) {
                    state = STATE_VERIFY_CHECKSUM;
                }
                break;

            case STATE_VERIFY_CHECKSUM:
                if (byte == computed_checksum) {
                    cout << "  ✅ [PACKET VERIFIED] Valid Frame Received! Cmd: 0x"
                         << hex << uppercase << (int)payload_buffer[0] << dec
                         << " | Payload Size: " << (int)payload_length << " bytes" << endl;
                } else {
                    cout << "  🚨 [CHECKSUM ERROR] Corrupted Packet Discarded!" << endl;
                }
                state = STATE_WAIT_HEADER1; // Reset to listen for next frame
                break;
        }
    }
};

int main() {
    cout << "--- DAY 32: UART SERIAL TELEMETRY FRAME PARSER (ESP32-CAM) ---" << endl << endl;

    UARTFrameParser parser;

    // Simulated noisy UART stream: [Garbage, Sync(0xAA, 0x55), Len(2), Cmd(0x20 = 'DRIVE'), Speed(0x50 = 80%), Checksum]
    // Checksum = Len(2) ^ Cmd(0x20) ^ Speed(0x50) = 2 ^ 32 ^ 80 = 114 (0x72)
    vector<uint8_t> raw_serial_stream = {
        0xFF, 0x12, /* Noise bytes */
        0xAA, 0x55, /* Valid Frame 1 Start */
        0x02,       /* Length: 2 bytes */
        0x20, 0x50, /* Payload: CMD=0x20, VAL=0x50 */
        0x72,       /* Correct Checksum (2 ^ 0x20 ^ 0x50 = 0x72) */
        0x00, 0x99, /* Inter-packet noise */
        0xAA, 0x55, /* Valid Frame 2 Start */
        0x01,       /* Length: 1 byte */
        0xEE,       /* Payload: 0xEE (E-STOP) */
        0x00        /* Wrong Checksum (Corrupted in transit) */
    };

    cout << "Feeding raw incoming byte stream into UART state machine:\n\n";

    for (size_t i = 0; i < raw_serial_stream.size(); i++) {
        uint8_t b = raw_serial_stream[i];
        parser.parseByte(b);
    }

    return 0;
}