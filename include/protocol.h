#pragma once

#include <stdint.h>

constexpr uint8_t LIGHT_HEADER = 0x8f; // Header for light data packet

// The light data packet from the PC to the light module.
struct __attribute__((packed)) LightData
{
    uint8_t header;
    uint8_t ID;       // Which segment the light is on, 0xff for cleaning
    uint8_t color[3]; // RGB color values, 0-255
    uint8_t checksum;
};
