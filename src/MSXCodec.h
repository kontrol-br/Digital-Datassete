#pragma once
#include "PlatformCodec.h"

class MSXCodec final : public PlatformCodec {
public:
    const char* name() const override;
    const char* folder() const override;
    bool playFile(const char* path) override;
    bool recordFile(const char* path) override;
};
