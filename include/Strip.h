#pragma once
#include <stdint.h>
namespace Strip
{
    void begin();

    void clear();

    void setActivePartition(uint8_t partition, uint32_t color);

    void setActivePartition(uint8_t partition, uint8_t r, uint8_t g, uint8_t b);

    void waterFlow(uint8_t blockSize);

} // namespace Strip
