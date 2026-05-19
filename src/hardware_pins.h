#pragma once
#include <stdint.h>

namespace Pins {
// SD SPI (SPI0 by default; easy to move to shared/separate SPI wiring).
constexpr uint8_t SD_CS = 21;
constexpr uint8_t SD_SCK = 25;
constexpr uint8_t SD_MOSI = 26;
constexpr uint8_t SD_MISO = 22;

// ST7735 display SPI (SPI1-style pinout by default).
constexpr uint8_t LCD_CS = 9;
constexpr uint8_t LCD_DC = 8;
constexpr uint8_t LCD_RST = 12;
constexpr uint8_t LCD_SCK = 10;
constexpr uint8_t LCD_MOSI = 11;

// Buttons, wired active-low with internal pull-ups.
constexpr uint8_t BTN_UP = 0;
constexpr uint8_t BTN_DOWN = 1;
constexpr uint8_t BTN_SELECT = 5;
constexpr uint8_t BTN_BACK = 6;
constexpr uint8_t BTN_PLAY_STOP = 4; // Optional, currently unused.

// Rotary encoder/menu aliases using the existing button inputs.
constexpr uint8_t ENCODER_LEFT = 1;
constexpr uint8_t ENCODER_RIGHT = 2;
constexpr uint8_t ENCODER_PUSH = 7;

// Optional SSD1306 I2C display wiring suggestion.
constexpr uint8_t OLED_SDA = 20;
constexpr uint8_t OLED_SCL = 21;

// Audio and capture pins.
constexpr uint8_t AUDIO_OUT_PIN = 15;
constexpr uint8_t DIGITAL_IN_PIN = 14;
constexpr uint8_t ADC_IN_PIN = 27; // GP27 / ADC1
constexpr uint8_t BUZZER_PIN = 13;
} // namespace Pins
