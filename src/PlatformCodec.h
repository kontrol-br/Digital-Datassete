#pragma once

class PlatformCodec {
public:
    virtual ~PlatformCodec() = default;
    virtual const char* name() const = 0;
    virtual const char* folder() const = 0;
    virtual bool playFile(const char* path) = 0;
    virtual bool recordFile(const char* path) = 0;
};
