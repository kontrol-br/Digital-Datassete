#include "AudioOutput.h"
#include "ArduinoCompat.h"
#include "hardware_pins.h"

void AudioOutput::begin() {
    pinMode(Pins::AUDIO_OUT_PIN, OUTPUT);
    digitalWrite(Pins::AUDIO_OUT_PIN, LOW);
}

void AudioOutput::setEnabled(bool enabled) {
    enabled_ = enabled;
    if (!enabled_) stop();
}

void AudioOutput::pulse(uint16_t halfPeriodUs) {
    if (!enabled_) return;
    state_ = !state_;
    digitalWrite(Pins::AUDIO_OUT_PIN, state_ ? HIGH : LOW);
    delayMicroseconds(halfPeriodUs);
}

void AudioOutput::stop() {
    state_ = false;
    digitalWrite(Pins::AUDIO_OUT_PIN, LOW);
}
