#include <Adafruit_NeoPixel.h>
#include "Strip.h"
#include "config.h"

namespace Strip
{
    static Adafruit_NeoPixel rgb;

    // These constants are defined in params.c
    // Will be patched by a script when flashing the firmware
    extern "C"
    {
        extern const uint8_t nPartitions;
        extern const uint8_t pixelsPerPartition;
        extern const uint8_t nPaddingPixels;
    };

    void begin()
    {
        rgb = Adafruit_NeoPixel(nPartitions * (pixelsPerPartition + nPaddingPixels) - nPaddingPixels, RGB_DATA_PIN, NEO_KHZ800 + NEO_BRG);
        rgb.begin();
    }

    void clear()
    {
        rgb.clear();
        rgb.show();
    }

    void setActivePartition(uint8_t partition, uint32_t color)
    {
        if (partition >= nPartitions)
            return;

        auto start = partition * (pixelsPerPartition + nPaddingPixels);
        rgb.clear();
        rgb.fill(color, start, pixelsPerPartition);
        rgb.show();
    }

    void setActivePartition(uint8_t partition, uint8_t r, uint8_t g, uint8_t b)
    {
        if (partition >= nPartitions)
            return;

        auto start = partition * (pixelsPerPartition + nPaddingPixels);
        rgb.clear();
        rgb.fill(rgb.Color(r, g, b), start, pixelsPerPartition);
        rgb.show();
    }
} // namespace Strip
