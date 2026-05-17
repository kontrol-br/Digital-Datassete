#include "ZX81Codec.h"
#include "ArduinoCompat.h"
#include "config.h"

const char* ZX81Codec::name() const { return "ZX81/TK85"; }
const char* ZX81Codec::folder() const { return Config::ZX81_FOLDER; }
bool ZX81Codec::playFile(const char* path) { Serial.print("ZX81 playback stub: "); Serial.println(path); return false; }
bool ZX81Codec::recordFile(const char* path) { Serial.print("ZX81 record stub: "); Serial.println(path); return false; }
