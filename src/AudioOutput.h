#pragma once
#include <stdint.h>

class AudioOutput {
public:
    void begin();
    void setEnabled(bool enabled);
    bool isEnabled() const { return enabled_; }
    void pulse(uint16_t halfPeriodUs);
    void stop();
private:
    bool enabled_ = false;
    bool state_ = false;
};
