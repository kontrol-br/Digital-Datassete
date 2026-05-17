#include "DisplayManager.h"
#include "ArduinoCompat.h"
#include "config.h"
#include "hardware_pins.h"
#include <stdio.h>
#ifndef UNIT_TEST
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
static Adafruit_ST7735 tft(Pins::LCD_CS, Pins::LCD_DC, Pins::LCD_MOSI, Pins::LCD_SCK, Pins::LCD_RST);
static constexpr uint16_t C_BLACK = ST77XX_BLACK;
static constexpr uint16_t C_WHITE = ST77XX_WHITE;
static constexpr uint16_t C_RED = ST77XX_RED;
static constexpr uint16_t C_GREEN = ST77XX_GREEN;
static constexpr uint16_t C_BLUE = ST77XX_BLUE;
static constexpr uint16_t C_YELLOW = ST77XX_YELLOW;
#else
static constexpr uint16_t C_BLACK=0, C_WHITE=1, C_RED=2, C_GREEN=3, C_BLUE=4, C_YELLOW=5;
struct DummyTft{void initR(int){} void setRotation(uint8_t){} void fillScreen(uint16_t){} void setTextColor(uint16_t){} void setTextSize(uint8_t){} void setCursor(int,int){} void print(const char*){} void print(int){} void print(float,int=2){} void println(const char*){} void fillRect(int,int,int,int,uint16_t){} void drawRect(int,int,int,int,uint16_t){} void drawFastHLine(int,int,int,uint16_t){}};
static DummyTft tft;
#define INITR_MINI160x80 0
#endif

bool DisplayManager::begin() {
    tft.initR(INITR_MINI160x80);
    setRotation(rotation_);
    tft.fillScreen(C_BLACK);
    return true;
}

void DisplayManager::setRotation(uint8_t rotation) {
    rotation_ = rotation % 4;
    tft.setRotation(rotation_);
}

void DisplayManager::splash() {
    tft.fillScreen(C_BLACK);
    tft.setTextColor(C_YELLOW);
    tft.setTextSize(1);
    tft.setCursor(8, 18); tft.println("Digital Datasette");
    tft.setTextColor(C_WHITE);
    tft.setCursor(18, 36); tft.println("RP2040 Pico");
    tft.setCursor(10, 54); tft.println("ZX/MSX/ZX81");
}

void DisplayManager::colorTest() {
    const uint16_t colors[] = {C_BLACK, C_RED, C_GREEN, C_BLUE, C_WHITE};
    for (uint8_t i = 0; i < 5; ++i) { tft.fillScreen(colors[i]); delay(120); }
    tft.fillScreen(C_BLACK);
    tft.drawRect(0, 0, Config::LCD_HEIGHT, Config::LCD_WIDTH, C_WHITE);
    tft.fillRect(4, 12, 30, 12, C_RED);
    tft.fillRect(38, 12, 30, 12, C_GREEN);
    tft.fillRect(72, 12, 30, 12, C_BLUE);
    tft.setCursor(6, 34); tft.setTextColor(C_WHITE); tft.println("Orientation OK?");
    delay(350);
}

void DisplayManager::showSelfTest(bool displayOk, bool adcOk, bool sdOk, bool buzzerOn) {
    tft.fillScreen(C_BLACK);
    tft.setTextColor(C_YELLOW); tft.setCursor(3, 3); tft.println("SELF TEST");
    tft.setTextColor(C_WHITE);
    tft.setCursor(3, 18); tft.print("Display "); tft.println(displayOk ? "OK" : "ERR");
    tft.setCursor(3, 30); tft.print("ADC     "); tft.println(adcOk ? "OK" : "ERR");
    tft.setCursor(3, 42); tft.print("SD      "); tft.println(sdOk ? "OK" : "ERR");
    tft.setCursor(3, 54); tft.print("Buzzer  "); tft.println(buzzerOn ? "ON" : "OFF");
}

void DisplayManager::showMenu(const char* title, const char* const* items, uint8_t count, uint8_t selected, const char* footer) {
    tft.fillScreen(C_BLACK);
    tft.setTextColor(C_YELLOW); tft.setCursor(0, 0); tft.println(title);
    tft.drawFastHLine(0, 10, Config::LCD_HEIGHT, C_BLUE);
    tft.setTextColor(C_WHITE);
    for (uint8_t i = 0; i < count && i < 5; ++i) {
        tft.setCursor(0, 15 + i * 11);
        tft.print(i == selected ? "> " : "  ");
        tft.println(items[i]);
    }
    if (footer) { tft.setCursor(0, 70); tft.setTextColor(C_BLUE); tft.println(footer); }
}

void DisplayManager::showMessage(const char* title, const char* line1, const char* line2, const char* footer) {
    tft.fillScreen(C_BLACK);
    tft.setTextColor(C_YELLOW); tft.setCursor(0, 0); tft.println(title);
    tft.setTextColor(C_WHITE); tft.setCursor(0, 18); if (line1) tft.println(line1);
    tft.setCursor(0, 32); if (line2) tft.println(line2);
    if (footer) { tft.setCursor(0, 68); tft.setTextColor(C_BLUE); tft.println(footer); }
}

void DisplayManager::showMonitoring(const InputLevel& level) {
    tft.fillScreen(C_BLACK);
    char buf[24];
    tft.setTextColor(C_YELLOW); tft.setCursor(0, 0); tft.println("INPUT MONITOR");
    tft.setTextColor(C_WHITE);
    tft.setCursor(0, 14); snprintf(buf, sizeof(buf), "ADC: %u", level.avgRaw); tft.println(buf);
    tft.setCursor(0, 26); snprintf(buf, sizeof(buf), "V: %.2fV", level.avgV); tft.println(buf);
    tft.setCursor(0, 38); snprintf(buf, sizeof(buf), "Vpp: %.2fV", level.vpp); tft.println(buf);
    const int barW = 72;
    int fill = static_cast<int>((level.vpp / Config::LEVEL_HIGH_MAX_VPP) * barW);
    if (fill < 0) fill = 0; if (fill > barW) fill = barW;
    tft.drawRect(0, 52, barW + 2, 10, C_WHITE);
    tft.fillRect(1, 53, fill, 8, level.status == LevelStatus::Clip ? C_RED : C_GREEN);
    tft.setCursor(78, 52); tft.println(InputMonitor::statusText(level.status));
    tft.setTextColor(C_BLUE); tft.setCursor(0, 70); tft.println("BACK to exit");
}

void DisplayManager::drawBrowser(const char* path, const char* const* entries, const bool* dirs, uint8_t count, uint8_t selected) {
    tft.fillScreen(C_BLACK);
    tft.setTextColor(C_YELLOW); tft.setCursor(0, 0); tft.println("SD Browser");
    tft.setTextColor(C_BLUE); tft.setCursor(0, 11); tft.println(path);
    tft.setTextColor(C_WHITE);
    for (uint8_t i = 0; i < count && i < 4; ++i) {
        tft.setCursor(0, 24 + i * 11);
        tft.print(i == selected ? "> " : "  ");
        if (dirs[i]) tft.print("[");
        tft.print(entries[i]);
        if (dirs[i]) tft.print("]");
    }
    tft.setTextColor(C_BLUE); tft.setCursor(0, 70); tft.println("OK open BACK back");
}
