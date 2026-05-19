#include "ArduinoCompat.h"
#include "AudioOutput.h"
#include "BuzzerManager.h"
#include "DisplayManager.h"
#include "InputMonitor.h"
#include "MSXCodec.h"
#include "MenuSystem.h"
#include "SdManager.h"
#include "ZXSpectrumCodec.h"
#include "ZX81Codec.h"
#include "config.h"
#include "hardware_pins.h"

DisplayManager display;
SdManager sd;
BuzzerManager buzzer;
InputMonitor inputMonitor;
AudioOutput audioOutput;
ZXSpectrumCodec zxSpectrum;
ZX81Codec zx81;
MSXCodec msx;
PlatformCodec* codecs[] = {&zxSpectrum, &zx81, &msx};
MenuSystem menu(display, sd, buzzer, inputMonitor, codecs, sizeof(codecs) / sizeof(codecs[0]));

static void initSystem() {
    Serial.begin(Config::SERIAL_BAUD);

    display.begin();
    inputMonitor.begin();

    pinMode(Pins::DIGITAL_IN_PIN, INPUT);
    audioOutput.begin();
    buzzer.begin();

    if (sd.begin()) {
        bool savedBuzzer = buzzer.isEnabled();
        if (sd.loadBuzzerEnabled(savedBuzzer)) buzzer.setEnabled(savedBuzzer);
        sd.ensurePlatformFolders();
    }
}

#ifndef UNIT_TEST
void setup() {
    initSystem();
    menu.begin();
}

void loop() {
    menu.update();
}
#else
int main() {
    initSystem();
    menu.begin();
    for (int i = 0; i < 2; ++i) menu.update();
    return 0;
}
#endif
