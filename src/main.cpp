#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "Strip.h"
#include "LoraModule.hpp"
#include "config.h"
#include "protocol.h"

inline uint8_t calculateChecksum(uint8_t *data, size_t size)
{
  uint8_t checksum = 0;
  for (size_t i = 0; i < size; i++)
  {
    checksum ^= data[i];
  }
  return checksum;
}

void setup()
{
  // put your setup code here, to run once:
  Serial1.setTx(UART1_TX_PIN);
  Serial1.setRx(UART1_RX_PIN);
  Serial1.begin(115200);

  ulog_init();
  ulog_subscribe([](ulog_level_t severity, char *msg)
                 { Serial1.printf("%d [%s]: %s\n", millis(), ulog_level_name(severity), msg); }, ULOG_DEBUG_LEVEL);

  LoraModule lora(Serial2, UART2_TX_PIN, UART2_RX_PIN, LORA_M0_PIN, LORA_M1_PIN);
  if (!lora.begin())
  {
    ULOG_ERROR("Lora module is fucked");
  }
  ULOG_INFO("Lora module is ready");

  Strip::begin();

  bool haveTarget = false;
  while (1)
  {
    if (Serial2.available())
    {
      if (Serial2.peek() != LIGHT_HEADER)
      {
        Serial2.read();
        continue;
      }

      LightData lightData;
      Serial2.readBytes(reinterpret_cast<uint8_t *>(&lightData),
                        sizeof(lightData));

      auto sum = calculateChecksum(reinterpret_cast<uint8_t *>(&lightData),
                                   sizeof(lightData));
      if (sum != 0)
      {
        ULOG_WARNING("Checksum error: %02X", sum);
        continue;
      }

      if (lightData.ID == 0xff)
      {
        haveTarget = false;
        ULOG_INFO("Cleaning");
        Strip::clear();
      }
      else
      {
        haveTarget = true;
        ULOG_INFO("Target: %u, Color: %u %u %u", lightData.ID,
                  lightData.color[0], lightData.color[1], lightData.color[2]);
        Strip::setActivePartition(lightData.ID, lightData.color[0],
                                  lightData.color[1], lightData.color[2]);
      }
    }

    if (!haveTarget)
    {
      Strip::waterFlow(3);
    }
    delay(20);
  }
}

void loop()
{
  // Never fuck with this
}
