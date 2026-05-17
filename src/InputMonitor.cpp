#include "InputMonitor.h"
#include "ArduinoCompat.h"
#include "config.h"
#include "hardware_pins.h"

void InputMonitor::begin() {
    analogReadResolution(12);
    pinMode(Pins::ADC_IN_PIN, INPUT);
}

InputLevel InputMonitor::readWindow(uint32_t windowMs) {
    const uint32_t start = millis();
    uint32_t sum = 0;
    uint32_t count = 0;
    uint16_t minRaw = Config::ADC_MAX;
    uint16_t maxRaw = 0;

    do {
        const uint16_t sample = static_cast<uint16_t>(analogRead(Pins::ADC_IN_PIN));
        if (sample < minRaw) minRaw = sample;
        if (sample > maxRaw) maxRaw = sample;
        sum += sample;
        ++count;
        delayMicroseconds(250);
    } while ((millis() - start) < windowMs);

    InputLevel level;
    level.minRaw = minRaw;
    level.maxRaw = maxRaw;
    level.avgRaw = count ? static_cast<uint16_t>(sum / count) : 0;
    level.minV = rawToVoltage(level.minRaw);
    level.maxV = rawToVoltage(level.maxRaw);
    level.avgV = rawToVoltage(level.avgRaw);
    level.vpp = level.maxV - level.minV;
    level.status = classify(level.minV, level.maxV, level.vpp);
    return level;
}

const char* InputMonitor::statusText(LevelStatus status) {
    switch (status) {
        case LevelStatus::Low: return "LOW";
        case LevelStatus::Good: return "GOOD";
        case LevelStatus::High: return "HIGH";
        case LevelStatus::Clip: return "CLIP";
    }
    return "?";
}

float InputMonitor::rawToVoltage(uint16_t raw) {
    return (static_cast<float>(raw) * Config::ADC_REF_VOLTAGE) / static_cast<float>(Config::ADC_MAX);
}

LevelStatus InputMonitor::classify(float minV, float maxV, float vpp) {
    if (minV < Config::CLIP_LOW_V || maxV > Config::CLIP_HIGH_V) return LevelStatus::Clip;
    if (vpp < Config::LEVEL_LOW_MAX_VPP) return LevelStatus::Low;
    if (vpp < Config::LEVEL_GOOD_MAX_VPP) return LevelStatus::Good;
    if (vpp < Config::LEVEL_HIGH_MAX_VPP) return LevelStatus::High;
    return LevelStatus::Clip;
}
