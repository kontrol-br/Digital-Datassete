#pragma once
#include "InputMonitor.h"
#include <stdint.h>

class DisplayManager {
public:
    bool begin();
    void setRotation(uint8_t rotation);
    void splash();
    void colorTest();
    void showSelfTest(bool displayOk, bool adcOk, bool sdOk, bool buzzerOn);
    void showMenu(const char* title, const char* const* items, uint8_t count, uint8_t selected, const char* footer = nullptr);
    void showMessage(const char* title, const char* line1, const char* line2 = nullptr, const char* footer = nullptr);
    void showMonitoring(const InputLevel& level);
    void drawBrowser(const char* path, const char* const* entries, const bool* dirs, uint8_t count, uint8_t selected);
private:
    uint8_t rotation_ = 3;
};
