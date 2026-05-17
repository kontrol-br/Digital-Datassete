#include "BuzzerManager.h"
#include "ArduinoCompat.h"
#include "hardware_pins.h"

void BuzzerManager::begin() {
    pinMode(Pins::BUZZER_PIN, OUTPUT);
    digitalWrite(Pins::BUZZER_PIN, LOW);
}

void BuzzerManager::setEnabled(bool enabled) {
    enabled_ = enabled;
    if (!enabled_) noTone(Pins::BUZZER_PIN);
}

void BuzzerManager::beep(uint16_t frequency, uint16_t durationMs) {
    if (!enabled_) return;
    tone(Pins::BUZZER_PIN, frequency, durationMs);
}

void BuzzerManager::click() { beep(2400, 18); }

void BuzzerManager::errorBeep() {
    if (!enabled_) return;
    beep(180, 100);
    delay(130);
    beep(180, 100);
}
