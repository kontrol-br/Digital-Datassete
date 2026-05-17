#pragma once
#include <stdint.h>

enum class LevelStatus { Low, Good, High, Clip };

struct InputLevel {
    uint16_t minRaw = 0;
    uint16_t maxRaw = 0;
    uint16_t avgRaw = 0;
    float minV = 0;
    float maxV = 0;
    float avgV = 0;
    float vpp = 0;
    LevelStatus status = LevelStatus::Low;
};

class InputMonitor {
public:
    void begin();
    InputLevel readWindow(uint32_t windowMs);
    static const char* statusText(LevelStatus status);
private:
    static float rawToVoltage(uint16_t raw);
    static LevelStatus classify(float minV, float maxV, float vpp);
};
