#pragma once
#include "PlatformCodec.h"
#include <stdint.h>

constexpr uint16_t ZX_PILOT_PULSE_US = 2168;
constexpr uint16_t ZX_SYNC1_US = 667;
constexpr uint16_t ZX_SYNC2_US = 735;
constexpr uint16_t ZX_BIT0_US = 855;
constexpr uint16_t ZX_BIT1_US = 1710;

class ZXSpectrumCodec final : public PlatformCodec {
public:
    const char* name() const override;
    const char* folder() const override;
    bool playFile(const char* path) override;
    bool recordFile(const char* path) override;
};
