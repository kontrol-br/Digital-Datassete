#pragma once
#include <stdint.h>

namespace Pins {
// SD SPI (SPI0 by default; easy to move to shared/separate SPI wiring).
constexpr uint8_t SD_CS = 17;
constexpr uint8_t SD_SCK = 18;
constexpr uint8_t SD_MOSI = 19;
constexpr uint8_t SD_MISO = 16;

// ST7735 display SPI (SPI1-style pinout by default).
constexpr uint8_t LCD_CS = 9;
constexpr uint8_t LCD_DC = 8;
constexpr uint8_t LCD_RST = 12;
constexpr uint8_t LCD_SCK = 10;
constexpr uint8_t LCD_MOSI = 11;

// Buttons, wired active-low with internal pull-ups.
constexpr uint8_t BTN_UP = 2;
constexpr uint8_t BTN_DOWN = 3;
constexpr uint8_t BTN_SELECT = 6;
constexpr uint8_t BTN_BACK = 7;
constexpr uint8_t BTN_PLAY_STOP = 4; // Optional, currently unused.

// Audio and capture pins.
constexpr uint8_t AUDIO_OUT_PIN = 15;
constexpr uint8_t DIGITAL_IN_PIN = 14;
constexpr uint8_t ADC_IN_PIN = 26; // GP26 / ADC0
constexpr uint8_t BUZZER_PIN = 13;
} // namespace Pins
