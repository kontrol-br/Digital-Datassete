#pragma once
#include <stdint.h>

namespace Config {
constexpr uint16_t LCD_WIDTH = 80;
constexpr uint16_t LCD_HEIGHT = 160;
constexpr uint8_t LCD_ROTATION = 1;
constexpr uint32_t SERIAL_BAUD = 115200;
constexpr uint16_t MENU_DEBOUNCE_MS = 160;
constexpr uint16_t MONITOR_REFRESH_MS = 100;
constexpr uint16_t MONITOR_WINDOW_MS = 30;
constexpr uint8_t MAX_DIR_ENTRIES = 32;
constexpr uint8_t MAX_PATH_LEN = 96;

constexpr float ADC_REF_VOLTAGE = 3.3f;
constexpr int ADC_MAX = 4095;
constexpr float ADC_DIVIDER_RATIO = 3.2f; // External circuit reconstruction factor.
constexpr float LEVEL_LOW_MAX_VPP = 0.15f;
constexpr float LEVEL_GOOD_MAX_VPP = 1.2f;
constexpr float LEVEL_HIGH_MAX_VPP = 2.6f;
constexpr float CLIP_LOW_V = 0.05f;
constexpr float CLIP_HIGH_V = 3.25f;

constexpr const char* ZX_SPECTRUM_FOLDER = "/ZX_SPECTRUM";
constexpr const char* ZX81_FOLDER = "/ZX81";
constexpr const char* MSX_FOLDER = "/MSX";
constexpr const char* CONFIG_FILE = "/CONFIG.INI";
} // namespace Config
