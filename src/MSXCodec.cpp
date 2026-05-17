#include "MSXCodec.h"
#include "ArduinoCompat.h"
#include "config.h"

const char* MSXCodec::name() const { return "MSX"; }
const char* MSXCodec::folder() const { return Config::MSX_FOLDER; }
bool MSXCodec::playFile(const char* path) { Serial.print("MSX playback stub: "); Serial.println(path); return false; }
bool MSXCodec::recordFile(const char* path) { Serial.print("MSX record stub: "); Serial.println(path); return false; }
