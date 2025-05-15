#include <stdint.h>

// These constants are used to define the network ID for the Lora module and params for RGB LED strip.
// Will be patched by a script when flashing the firmware
const uint8_t netID = 11; // Network ID of the nodes

const uint8_t nPartitions = 5;         // Number of partitions of tennis net
const uint8_t pixelsPerPartition = 50; // Number of pixels per partition
const uint8_t nPaddingPixels = 5;       // Number of padding pixels between partitions