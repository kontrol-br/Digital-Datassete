#pragma once
#include <stdint.h>

class BuzzerManager {
public:
    void begin();
    void setEnabled(bool enabled);
    bool isEnabled() const { return enabled_; }
    void beep(uint16_t frequency, uint16_t durationMs);
    void click();
    void errorBeep();
private:
    bool enabled_ = true;
};
