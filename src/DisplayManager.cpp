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
struct DummyTft{void initR(int){} void setRotation(uint8_t){} void fillScreen(uint16_t){} void setTextColor(uint16_t){} void setTextSize(uint8_t){} void setCursor(int,int){} void print(const char*){} void print(int){} void print(float,int=2){} void println(const char*){} void fillRect(int,int,int,int,uint16_t){} void drawRect(int,int,int,int,uint16_t){} void drawFastHLine(int,int,int,uint16_t){} void invertDisplay(bool){}};
static DummyTft tft;
#define INITR_MINI160x80 0
#endif

namespace {
constexpr int16_t UI_OFFSET_X = 3;
constexpr int16_t UI_OFFSET_Y = 3;

int16_t sx(int16_t x) { return x + UI_OFFSET_X; }
int16_t sy(int16_t y) { return y + UI_OFFSET_Y; }

void drawBoldText(int16_t x, int16_t y, const char* text, uint16_t color) {
    tft.setTextColor(color);
    tft.setCursor(sx(x), sy(y));
    tft.print(text);
    tft.setCursor(sx(x + 1), sy(y));
    tft.print(text);
}
}

bool DisplayManager::begin() {
    tft.initR(INITR_MINI160x80);
    setRotation(rotation_);
    tft.invertDisplay(true);
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
    tft.setCursor(sx(8), sy(18)); tft.println("Digital Datasette");
    tft.setTextColor(C_WHITE);
    tft.setCursor(sx(18), sy(36)); tft.println("RP2040 Pico");
    tft.setCursor(sx(10), sy(54)); tft.println("ZX/MSX/ZX81");
}

void DisplayManager::colorTest() {
    // Orientation screen intentionally disabled.
    // Keep method as a no-op to preserve interface compatibility.
}

void DisplayManager::showSelfTest(bool displayOk, bool adcOk, bool sdOk, bool buzzerOn) {
    tft.fillScreen(C_BLACK);
    tft.setTextColor(C_YELLOW); tft.setCursor(sx(3), sy(3)); tft.println("SELF TEST");
    tft.setTextColor(C_WHITE);
    tft.setCursor(sx(3), sy(18)); tft.print("Display "); tft.println(displayOk ? "OK" : "ERR");
    tft.setCursor(sx(3), sy(30)); tft.print("ADC     "); tft.println(adcOk ? "OK" : "ERR");
    tft.setCursor(sx(3), sy(42)); tft.print("SD      "); tft.println(sdOk ? "OK" : "ERR");
    tft.setCursor(sx(3), sy(54)); tft.print("Buzzer  "); tft.println(buzzerOn ? "ON" : "OFF");
}

void DisplayManager::showMenu(const char* title, const char* const* items, uint8_t count, uint8_t selected, const char* footer) {
    static const char* lastTitle = nullptr;
    static const char* const* lastItems = nullptr;
    static uint8_t lastCount = 0;
    static const char* lastFooter = nullptr;
    static int16_t lastSelected = -1;

    const bool fullRedraw = (lastTitle != title) || (lastItems != items) || (lastCount != count) || (lastFooter != footer) ||
                            (selected >= count) || (lastSelected < 0) || (lastSelected >= count);

    if (fullRedraw) {
        tft.fillScreen(C_BLACK);
        drawBoldText(0, 0, title, C_YELLOW);
        tft.drawFastHLine(sx(0), sy(10), Config::LCD_HEIGHT - UI_OFFSET_X, C_BLUE);
        tft.setTextColor(C_WHITE);
        for (uint8_t i = 0; i < count && i < 5; ++i) {
            tft.setCursor(sx(0), sy(15 + i * 11));
            if (i == selected) { tft.setTextColor(C_YELLOW); tft.print("o "); tft.setTextColor(C_WHITE); }
            else tft.print("  ");
            tft.println(items[i]);
        }
        if (footer) { tft.setCursor(sx(0), sy(70)); tft.setTextColor(C_BLUE); tft.println(footer); }
    } else if (lastSelected != selected) {
        const uint8_t oldSel = static_cast<uint8_t>(lastSelected);
        const uint8_t maxVisible = count > 5 ? 5 : count;
        if (oldSel < maxVisible) {
            tft.fillRect(sx(0), sy(15 + oldSel * 11), Config::LCD_HEIGHT - UI_OFFSET_X, 9, C_BLACK);
            tft.setCursor(sx(0), sy(15 + oldSel * 11));
            tft.setTextColor(C_WHITE);
            tft.print("  ");
            tft.println(items[oldSel]);
        }
        if (selected < maxVisible) {
            tft.fillRect(sx(0), sy(15 + selected * 11), Config::LCD_HEIGHT - UI_OFFSET_X, 9, C_BLACK);
            tft.setCursor(sx(0), sy(15 + selected * 11));
            tft.setTextColor(C_YELLOW);
            tft.print("o ");
            tft.setTextColor(C_WHITE);
            tft.println(items[selected]);
        }
    }

    lastTitle = title;
    lastItems = items;
    lastCount = count;
    lastFooter = footer;
    lastSelected = selected;
}

void DisplayManager::showMessage(const char* title, const char* line1, const char* line2, const char* footer) {
    tft.fillScreen(C_BLACK);
    drawBoldText(0, 0, title, C_YELLOW);
    tft.setTextColor(C_WHITE); tft.setCursor(sx(0), sy(18)); if (line1) tft.println(line1);
    tft.setCursor(sx(0), sy(32)); if (line2) tft.println(line2);
    if (footer) { tft.setCursor(sx(0), sy(68)); tft.setTextColor(C_BLUE); tft.println(footer); }
}

void DisplayManager::showMonitoring(const InputLevel& level) {
    tft.fillScreen(C_BLACK);
    char buf[24];
    tft.setTextColor(C_YELLOW); tft.setCursor(sx(0), sy(0)); tft.println("INPUT MONITOR");
    tft.setTextColor(C_WHITE);
    tft.setCursor(sx(0), sy(14)); snprintf(buf, sizeof(buf), "ADC: %u", level.avgRaw); tft.println(buf);
    tft.setCursor(sx(0), sy(26)); snprintf(buf, sizeof(buf), "V: %.2fV", level.avgV); tft.println(buf);
    tft.setCursor(sx(0), sy(38)); snprintf(buf, sizeof(buf), "Vpp: %.2fV", level.vpp); tft.println(buf);
    const int barW = 72;
    int fill = static_cast<int>((level.vpp / Config::LEVEL_HIGH_MAX_VPP) * barW);
    if (fill < 0) fill = 0; if (fill > barW) fill = barW;
    tft.drawRect(sx(0), sy(52), barW + 2, 10, C_WHITE);
    tft.fillRect(sx(1), sy(53), fill, 8, level.status == LevelStatus::Clip ? C_RED : C_GREEN);
    tft.setCursor(sx(78), sy(52)); tft.println(InputMonitor::statusText(level.status));
    tft.setTextColor(C_BLUE); tft.setCursor(sx(0), sy(70)); tft.println("BACK to exit");
}

void DisplayManager::drawBrowser(const char* path, const char* const* entries, const bool* dirs, uint8_t count, uint8_t selected) {
    tft.fillScreen(C_BLACK);
    drawBoldText(0, 0, "SD Browser", C_YELLOW);
    tft.setTextColor(C_BLUE); tft.setCursor(sx(0), sy(11)); tft.println(path);
    tft.setTextColor(C_WHITE);
    for (uint8_t i = 0; i < count && i < 4; ++i) {
        tft.setCursor(sx(0), sy(24 + i * 11));
        if (i == selected) { tft.setTextColor(C_YELLOW); tft.print("o "); tft.setTextColor(C_WHITE); }
        else tft.print("  ");
        if (dirs[i]) tft.print("[");
        tft.print(entries[i]);
        if (dirs[i]) tft.print("]");
    }
    tft.setTextColor(C_BLUE); tft.setCursor(sx(0), sy(70)); tft.println("OK open BACK back");
}
