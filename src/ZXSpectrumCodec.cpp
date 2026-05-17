#include "ZXSpectrumCodec.h"
#include "ArduinoCompat.h"
#include "config.h"

const char* ZXSpectrumCodec::name() const { return "ZX Spectrum/TK90X"; }
const char* ZXSpectrumCodec::folder() const { return Config::ZX_SPECTRUM_FOLDER; }
bool ZXSpectrumCodec::playFile(const char* path) {
    Serial.print("ZX Spectrum playback stub: ");
    Serial.println(path);
    return false;
}
bool ZXSpectrumCodec::recordFile(const char* path) {
    Serial.print("ZX Spectrum record stub: ");
    Serial.println(path);
    return false;
}
