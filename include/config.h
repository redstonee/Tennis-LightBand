#pragma once

#include <Arduino.h>

constexpr auto RGB_DATA_PIN = PB11;

// LoRa pins
constexpr auto LORA_M0_PIN = PA0;
constexpr auto LORA_M1_PIN = PA1;
constexpr auto UART2_TX_PIN = PA2;
constexpr auto UART2_RX_PIN = PA3;

// Debug UART
constexpr auto UART1_TX_PIN = PA9;
constexpr auto UART1_RX_PIN = PA10;

constexpr uint8_t LORA_CHANNEL = 45;           // Channel of the nodes
