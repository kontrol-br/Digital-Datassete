#pragma once
#include "config.h"
#include <stdint.h>

struct DirEntry {
    char name[32] = {0};
    bool isDir = false;
};

class SdManager {
public:
    bool begin();
    bool isReady() const { return ready_; }
    bool ensurePlatformFolders();
    uint8_t listDir(const char* path, DirEntry* entries, uint8_t maxEntries);
    bool removeFile(const char* path);
    bool makeDir(const char* path);
    bool loadBuzzerEnabled(bool& enabled);
    bool saveBuzzerEnabled(bool enabled);
private:
    bool ready_ = false;
};
