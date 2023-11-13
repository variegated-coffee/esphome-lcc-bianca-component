//
// Created by Magnus Nordlander on 2023-09-30.
//

#include "OpenLCCBianca.h"
#include "esphome/components/logger/logger.h"

namespace esphome {
    namespace open_lcc_bianca {
        void OpenLCCBianca::update() {

        }

        void OpenLCCBianca::setup() {
//        messageBuffer = malloc()
        }

        void OpenLCCBianca::loop() {
            if (enabled && this->available() >= (sizeof(ESPMessageHeader) + sizeof(ESPSystemStatusMessage))) {
                uint8_t peek;
                this->peek_byte(&peek);
                if (peek != 0x01) {
                    this->read_byte(&peek);
                    if (this->available() < (sizeof(ESPMessageHeader) + sizeof(ESPSystemStatusMessage))) {
                        return;
                    }
                }

                ESPMessageHeader header{};
                bool success = this->read_array(reinterpret_cast<uint8_t *>(&header), sizeof(ESPMessageHeader));

                if (!success) {
                    ESP_LOGD("LCC", "Unsuccessful read");
                    return;
                }

                ESP_LOGV("LCC", "Successful read");
                ESP_LOGV("LCC", "Direction: %lu", header.direction);
                ESP_LOGV("LCC", "Id: %lu", header.id);
                ESP_LOGV("LCC", "Response to: %lu", header.responseTo);
                ESP_LOGV("LCC", "Type: %lu", header.type);
                ESP_LOGV("LCC", "Length: %lu", header.length);

                if (header.error > ESP_DEBUG_LEVEL) {
                    ESP_LOGD("LCC", "Debug error: %xu", header.error);
                } else if (header.error > ESP_WARNING_LEVEL) {
                    ESP_LOGW("LCC", "Warning: %xu", header.error);
                } else if (header.error != ESP_ERROR_NONE) {
                    ESP_LOGE("LCC", "Error: %xu", header.error);
                }

                switch (header.type) {
                    case ESP_MESSAGE_PING:
                        return handlePingMessage(&header);
                    case ESP_MESSAGE_SYSTEM_STATUS:
                        return handleSystemStatusMessage(&header);
                    case ESP_MESSAGE_NACK:
                        return handleNack(&header);
                    case ESP_MESSAGE_ACK:
                        break;
                    case ESP_MESSAGE_PONG:
                        break;
//                case ESP_MESSAGE_ESP_STATUS:
                    case ESP_MESSAGE_SYSTEM_COMMAND:
                    default:
                        break;
                }
            }
        }

        void OpenLCCBianca::handleNack(ESPMessageHeader *header) {
        }

        void OpenLCCBianca::handlePingMessage(ESPMessageHeader *header) {
            if (header->length == sizeof(ESPPingMessage)) {
                ESPPingMessage message{};
                bool success = this->read_array(reinterpret_cast<uint8_t *>(&message), sizeof(message));

                if (success) {
                    if (message.version == 0x0001) {
                        ESPMessageHeader responseHeader{
                                .direction = ESP_DIRECTION_ESP32_TO_RP2040,
                                .id = static_cast<uint16_t>(esp_random()),
                                .responseTo = header->id,
                                .type = ESP_MESSAGE_PONG,
                                .error = ESP_ERROR_NONE,
                                .version = ESP_RP2040_PROTOCOL_VERSION,
                                .length = sizeof(ESPPongMessage),
                        };
                        ESPPongMessage responseMessage{};

                        this->write_array(reinterpret_cast<const uint8_t *>(&responseHeader), sizeof(ESPMessageHeader));
                        this->write_array(reinterpret_cast<const uint8_t *>(&responseMessage), sizeof(ESPPongMessage));
                    } else {
                        ESPMessageHeader responseHeader{
                                .direction = ESP_DIRECTION_ESP32_TO_RP2040,
                                .id = static_cast<uint16_t>(esp_random()),
                                .responseTo = header->id,
                                .type = ESP_MESSAGE_NACK,
                                .error = ESP_ERROR_PING_WRONG_VERSION,
                                .version = ESP_RP2040_PROTOCOL_VERSION,
                                .length = 0,
                        };

                        this->write_array(reinterpret_cast<const uint8_t *>(&responseHeader), sizeof(ESPMessageHeader));
                    }
                } else {
                    nackMessage(header);
                }
            }
        }

        void OpenLCCBianca::handleSystemStatusMessage(ESPMessageHeader *header) {
            ESP_LOGV("LCC", "Handling system status");
            if (header->length == sizeof(ESPSystemStatusMessage)) {
                ESPSystemStatusMessage message{};
                bool success = this->read_array(reinterpret_cast<uint8_t *>(&message), sizeof(message));

                if (success) {
                    ackMessage(header);

//                    ESP_LOGD("LCC", "SB High: %u", message.sbRawHi);
//                    ESP_LOGD("LCC", "SB Low: %u", message.sbRawLo);

                    this->on_status_update_callback_.call(message);
                } else {

                    nackMessage(header);
                }
            } else {
                ESP_LOGD("LCC", "Wrong length, expected %u, was %u", sizeof(ESPSystemStatusMessage), header->length);
            }
        }

        void OpenLCCBianca::ackMessage(ESPMessageHeader *header) {
            ESPMessageHeader responseHeader{
                    .direction = ESP_DIRECTION_ESP32_TO_RP2040,
                    .id = static_cast<uint16_t>(esp_random()),
                    .responseTo = header->id,
                    .type = ESP_MESSAGE_ACK,
                    .error = ESP_ERROR_NONE,
                    .version = ESP_RP2040_PROTOCOL_VERSION,
                    .length = 0,
            };

            this->write_array(reinterpret_cast<const uint8_t *>(&responseHeader), sizeof(ESPMessageHeader));
        }

        void OpenLCCBianca::nackMessage(ESPMessageHeader *header) {
            ESPMessageHeader responseHeader{
                    .direction = ESP_DIRECTION_ESP32_TO_RP2040,
                    .id = esp_random(),
                    .responseTo = header->id,
                    .type = ESP_MESSAGE_NACK,
                    .error = ESP_ERROR_UNEXPECTED_MESSAGE_LENGTH,
                    .version = ESP_RP2040_PROTOCOL_VERSION,
                    .length = 0,
            };

            this->write_array(reinterpret_cast<const uint8_t *>(&responseHeader), sizeof(ESPMessageHeader));
        }

        void OpenLCCBianca::sendCommand(ESPSystemCommandType type, bool boolArg) {
            ESPSystemCommandPayload payload{
                    .type = type,
                    .bool1 = boolArg,
                    .float1 = 0.0f,
                    .float2 = 0.0f,
                    .float3 = 0.0f,
                    .float4 = 0.0f,
                    .float5 = 0.0f,
                    .int1 = 0x00000000,
                    .int2 = 0x00000000,
                    .int3 = 0x00000000,
            };

            ESP_LOGD("LCC", "Sending Command of type %u with arg %u", payload.type, boolArg);

            sendCommand(payload);
        }

        void OpenLCCBianca::sendCommand(ESPSystemCommandType type, float floatArg) {
            ESPSystemCommandPayload payload{
                    .type = type,
                    .bool1 = false,
                    .float1 = floatArg,
                    .float2 = 0.0f,
                    .float3 = 0.0f,
                    .float4 = 0.0f,
                    .float5 = 0.0f,
                    .int1 = 0x00000000,
                    .int2 = 0x00000000,
                    .int3 = 0x00000000,
            };

            ESP_LOGD("LCC", "Sending Command of type %u with arg %f", payload.type, floatArg);

            sendCommand(payload);
        }

        void OpenLCCBianca::sendCommand(ESPSystemCommandType type, uint32_t intArg) {
            ESPSystemCommandPayload payload{
                    .type = type,
                    .bool1 = false,
                    .float1 = 0.0f,
                    .float2 = 0.0f,
                    .float3 = 0.0f,
                    .float4 = 0.0f,
                    .float5 = 0.0f,
                    .int1 = intArg,
                    .int2 = 0x00000000,
                    .int3 = 0x00000000,
            };

            ESP_LOGD("LCC", "Sending Command of type %u with arg %x", payload.type, intArg);

            sendCommand(payload);
        }

        void OpenLCCBianca::sendCommand(ESPSystemCommandPayload payload) {
            ESP_LOGD("LCC", "Sending Command of type %u", payload.type);

            ESPMessageHeader commandHeader{
                    .direction = ESP_DIRECTION_ESP32_TO_RP2040,
                    .id = esp_random(),
                    .responseTo = 0,
                    .type = ESP_MESSAGE_SYSTEM_COMMAND,
                    .error = ESP_ERROR_NONE,
                    .version = ESP_RP2040_PROTOCOL_VERSION,
                    .length = sizeof(ESPSystemCommandMessage),
            };

            ESPSystemCommandMessage commandMessage{
                    .checksum = crc32(&payload, sizeof(ESPSystemCommandPayload)),
                    .payload = payload,
            };

            this->write_array(reinterpret_cast<const uint8_t *>(&commandHeader), sizeof(ESPMessageHeader));
            this->write_array(reinterpret_cast<const uint8_t *>(&commandMessage), sizeof(ESPSystemCommandMessage));
        }

        uint32_t OpenLCCBianca::crc32_for_byte(uint32_t r) {
            for (int j = 0; j < 8; ++j)
                r = (r & 1 ? 0 : (uint32_t) 0xEDB88320L) ^ r >> 1;
            return r ^ (uint32_t) 0xFF000000L;
        }

        uint32_t OpenLCCBianca::crc32(const void *data, size_t n_bytes) {
            uint32_t crc = 0;
            static uint32_t table[0x100];
            if (!*table)
                for (size_t i = 0; i < 0x100; ++i)
                    table[i] = crc32_for_byte(i);
            for (size_t i = 0; i < n_bytes; ++i)
                crc = table[(uint8_t) crc ^ ((uint8_t *) data)[i]] ^ crc >> 8;

            return crc;
        }
    }
}