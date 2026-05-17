#include "SdManager.h"
#include "ArduinoCompat.h"
#include "hardware_pins.h"
#include <string.h>
#ifndef UNIT_TEST
#include <SD.h>
#include <SPI.h>
#endif

bool SdManager::begin() {
#ifndef UNIT_TEST
    SPI.setRX(Pins::SD_MISO);
    SPI.setTX(Pins::SD_MOSI);
    SPI.setSCK(Pins::SD_SCK);
    ready_ = SD.begin(Pins::SD_CS);
#else
    ready_ = false;
#endif
    Serial.println(ready_ ? "SD OK" : "SD unavailable");
    return ready_;
}

bool SdManager::ensurePlatformFolders() {
    if (!ready_) return false;
    return makeDir(Config::ZX_SPECTRUM_FOLDER) && makeDir(Config::ZX81_FOLDER) && makeDir(Config::MSX_FOLDER);
}

bool SdManager::makeDir(const char* path) {
    if (!ready_) return false;
#ifndef UNIT_TEST
    if (SD.exists(path)) return true;
    return SD.mkdir(path);
#else
    (void)path; return false;
#endif
}

uint8_t SdManager::listDir(const char* path, DirEntry* entries, uint8_t maxEntries) {
    if (!ready_ || !entries || !maxEntries) return 0;
    uint8_t count = 0;
    if (strcmp(path, "/") != 0) {
        strncpy(entries[count].name, "..", sizeof(entries[count].name) - 1);
        entries[count].isDir = true;
        ++count;
    }
#ifndef UNIT_TEST
    File dir = SD.open(path);
    if (!dir || !dir.isDirectory()) return count;
    File file = dir.openNextFile();
    while (file && count < maxEntries) {
        const char* raw = file.name();
        const char* slash = strrchr(raw, '/');
        const char* name = slash ? slash + 1 : raw;
        if (name[0] != '.') {
            strncpy(entries[count].name, name, sizeof(entries[count].name) - 1);
            entries[count].name[sizeof(entries[count].name) - 1] = '\0';
            entries[count].isDir = file.isDirectory();
            ++count;
        }
        file.close();
        file = dir.openNextFile();
    }
    dir.close();
#endif
    return count;
}

bool SdManager::removeFile(const char* path) {
    if (!ready_) return false;
#ifndef UNIT_TEST
    return SD.remove(path);
#else
    (void)path; return false;
#endif
}

bool SdManager::loadBuzzerEnabled(bool& enabled) {
    if (!ready_) return false;
#ifndef UNIT_TEST
    File file = SD.open(Config::CONFIG_FILE, FILE_READ);
    if (!file) return false;
    String data = file.readString();
    file.close();
    if (data.indexOf("buzzer=0") >= 0) enabled = false;
    if (data.indexOf("buzzer=1") >= 0) enabled = true;
    return true;
#else
    (void)enabled; return false;
#endif
}

bool SdManager::saveBuzzerEnabled(bool enabled) {
    if (!ready_) return false;
#ifndef UNIT_TEST
    File file = SD.open(Config::CONFIG_FILE, FILE_WRITE);
    if (!file) return false;
    file.seek(0);
    file.print("buzzer=");
    file.println(enabled ? "1" : "0");
    file.close();
    return true;
#else
    (void)enabled; return false;
#endif
}
