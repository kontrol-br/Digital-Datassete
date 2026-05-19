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

static void runSelfTest() {
    Serial.begin(Config::SERIAL_BAUD);
    delay(250);
    Serial.println("Digital Datasette boot");

    const bool displayOk = display.begin();
    display.splash();
    delay(450);
    display.colorTest();

    inputMonitor.begin();
    const bool adcOk = true;

    pinMode(Pins::DIGITAL_IN_PIN, INPUT);
    audioOutput.begin();
    buzzer.begin();
    buzzer.beep(1800, 70);

    const bool sdOk = sd.begin();
    if (sdOk) {
        bool savedBuzzer = buzzer.isEnabled();
        if (sd.loadBuzzerEnabled(savedBuzzer)) buzzer.setEnabled(savedBuzzer);
        sd.ensurePlatformFolders();
    }

    display.showSelfTest(displayOk, adcOk, sdOk, buzzer.isEnabled());
    delay(1100);
}

#ifndef UNIT_TEST
void setup() {
    runSelfTest();
    menu.begin();
}

void loop() {
    menu.update();
}
#else
int main() {
    runSelfTest();
    menu.begin();
    for (int i = 0; i < 2; ++i) menu.update();
    return 0;
}
#endif
