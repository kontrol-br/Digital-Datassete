#include "MenuSystem.h"
#include "ArduinoCompat.h"
#include "config.h"
#include "hardware_pins.h"
#include <string.h>

MenuSystem::MenuSystem(DisplayManager& display, SdManager& sd, BuzzerManager& buzzer, InputMonitor& monitor,
                       PlatformCodec** codecs, uint8_t codecCount)
    : display_(display), sd_(sd), buzzer_(buzzer), monitor_(monitor), codecs_(codecs), codecCount_(codecCount) {}

void MenuSystem::begin() {
    pinMode(Pins::ENCODER_LEFT, INPUT_PULLUP);
    pinMode(Pins::ENCODER_RIGHT, INPUT_PULLUP);
    pinMode(Pins::ENCODER_PUSH, INPUT_PULLUP);
    pinMode(Pins::BTN_BACK, INPUT_PULLUP);
    render();
}

void MenuSystem::update() {
    if (screen_ == Screen::Monitoring && millis() - lastMonitorMs_ >= Config::MONITOR_REFRESH_MS) {
        lastMonitorMs_ = millis();
        display_.showMonitoring(monitor_.readWindow(Config::MONITOR_WINDOW_MS));
    }
    if (screen_ == Screen::Message && millis() > messageUntilMs_) {
        screen_ = returnScreen_;
        render();
    }
    const Button button = readButton();
    if (button == Button::None) return;
    buzzer_.click();
    switch (button) {
        case Button::Up: move(-1, 0); break;
        case Button::Down: move(1, 0); break;
        case Button::Select: select(); break;
        case Button::Back: back(); break;
        case Button::None: break;
    }
}

MenuSystem::Button MenuSystem::readButton() {
    static uint8_t lastClkState = HIGH;
    if (millis() - lastButtonMs_ < Config::MENU_DEBOUNCE_MS) return Button::None;

    if (digitalRead(Pins::BTN_UP) == LOW) {
        lastButtonMs_ = millis();
        return Button::Up;
    }
    if (digitalRead(Pins::BTN_DOWN) == LOW) {
        lastButtonMs_ = millis();
        return Button::Down;
    }

    if (digitalRead(Pins::ENCODER_PUSH) == LOW || digitalRead(Pins::BTN_SELECT) == LOW) {
        lastButtonMs_ = millis();
        return Button::Select;
    }
    if (digitalRead(Pins::BTN_BACK) == LOW) {
        lastButtonMs_ = millis();
        return Button::Back;
    }

    const uint8_t clkState = digitalRead(Pins::ENCODER_LEFT);
    const uint8_t dtState = digitalRead(Pins::ENCODER_RIGHT);
    if (lastClkState == LOW && clkState == HIGH) {
        lastButtonMs_ = millis();
        lastClkState = clkState;
        return (dtState == LOW) ? Button::Down : Button::Up;
    }

    lastClkState = clkState;
    return Button::None;
}

void MenuSystem::move(int delta, uint8_t count) {
    if (screen_ == Screen::Main) count = 5;
    else if (screen_ == Screen::Platform) count = 3;
    else if (screen_ == Screen::Tools) count = 3;
    else if (screen_ == Screen::Settings) count = 3;
    else if (screen_ == Screen::Browser) count = entryCount_;
    else if (screen_ == Screen::FileActions) count = 3;
    else if (screen_ == Screen::DeleteConfirm) count = 2;
    if (!count) return;
    selected_ = (selected_ + count + delta) % count;
    render();
}

void MenuSystem::select() {
    if (screen_ == Screen::Main) {
        if (selected_ < codecCount_) { activeCodec_ = selected_; screen_ = Screen::Platform; selected_ = 0; }
        else if (selected_ == 3) { screen_ = Screen::Tools; selected_ = 0; }
        else { screen_ = Screen::Settings; selected_ = 0; }
        render(); return;
    }
    if (screen_ == Screen::Platform) {
        if (selected_ == 0) enterBrowser(codecs_[activeCodec_]->folder());
        else if (selected_ == 1) showTransient("Record", "Placeholder", "Capture soon");
        else back();
        return;
    }
    if (screen_ == Screen::Tools) {
        if (selected_ == 0) { screen_ = Screen::Monitoring; lastMonitorMs_ = 0; render(); }
        else if (selected_ == 1) enterBrowser("/");
        else back();
        return;
    }
    if (screen_ == Screen::Settings) {
        if (selected_ == 0) { buzzer_.setEnabled(!buzzer_.isEnabled()); sd_.saveBuzzerEnabled(buzzer_.isEnabled()); render(); }
        else if (selected_ == 1) showTransient("Display", "Rotation hook", "Edit config.h");
        else back();
        return;
    }
    if (screen_ == Screen::Browser) {
        if (!sd_.isReady()) { showTransient("SD Browser", "SD unavailable"); return; }
        if (entryCount_ == 0) return;
        const DirEntry& e = entries_[selected_];
        if (strcmp(e.name, "..") == 0) { parentPath(browserPath_); refreshBrowser(); render(); return; }
        char full[Config::MAX_PATH_LEN]; joinPath(full, sizeof(full), browserPath_, e.name);
        if (e.isDir) enterBrowser(full);
        else { strncpy(selectedFile_, full, sizeof(selectedFile_) - 1); selectedFile_[sizeof(selectedFile_) - 1] = '\0'; screen_ = Screen::FileActions; selected_ = 0; render(); }
        return;
    }
    if (screen_ == Screen::FileActions) {
        if (selected_ == 0) { codecs_[activeCodec_]->playFile(selectedFile_); showTransient("Play", "Codec stub", selectedFile_); }
        else if (selected_ == 1) { screen_ = Screen::DeleteConfirm; selected_ = 0; render(); }
        else back();
        return;
    }
    if (screen_ == Screen::DeleteConfirm) {
        if (selected_ == 1) {
            const bool ok = sd_.removeFile(selectedFile_);
            refreshBrowser();
            showTransient("Delete", ok ? "File deleted" : "Delete failed");
        } else { screen_ = Screen::FileActions; selected_ = 0; render(); }
    }
}

void MenuSystem::back() {
    if (screen_ == Screen::Main) return;
    if (screen_ == Screen::Browser) { screen_ = browserReturnScreen_; selected_ = 0; render(); return; }
    if (screen_ == Screen::FileActions || screen_ == Screen::DeleteConfirm) { screen_ = Screen::Browser; selected_ = 0; refreshBrowser(); render(); return; }
    screen_ = Screen::Main;
    selected_ = 0;
    render();
}

void MenuSystem::render() {
    static const char* mainItems[] = {"ZX Spectrum/TK90X", "ZX81/TK85", "MSX", "Tools", "Settings"};
    static const char* tools[] = {"Monitoring", "SD Browser", "Back"};
    static const char* fileActions[] = {"Play", "Delete", "Back"};
    static const char* confirm[] = {"No", "Yes"};
    if (screen_ == Screen::Main) display_.showMenu("Select Platform", mainItems, 5, selected_);
    else if (screen_ == Screen::Platform) {
        static const char* spectrum[] = {"Play from SD", "Record TAP/TZX", "Back"};
        static const char* zx81[] = {"Play from SD", "Record to P", "Back"};
        static const char* msx[] = {"Play from SD", "Record CAS", "Back"};
        const char* const* items = activeCodec_ == 0 ? spectrum : (activeCodec_ == 1 ? zx81 : msx);
        display_.showMenu(codecs_[activeCodec_]->name(), items, 3, selected_);
    } else if (screen_ == Screen::Tools) display_.showMenu("Tools", tools, 3, selected_);
    else if (screen_ == Screen::Settings) {
        static char buzzer[16]; snprintf(buzzer, sizeof(buzzer), "Buzzer: %s", buzzer_.isEnabled() ? "ON" : "OFF");
        const char* settings[] = {buzzer, "Display Rotation", "Back"};
        display_.showMenu("Settings", settings, 3, selected_);
    } else if (screen_ == Screen::Monitoring) display_.showMonitoring(monitor_.readWindow(Config::MONITOR_WINDOW_MS));
    else if (screen_ == Screen::Browser) {
        if (!sd_.isReady()) display_.showMessage("SD Browser", "SD unavailable", nullptr, "BACK to exit");
        else {
            uint8_t start = selected_ > 3 ? selected_ - 3 : 0;
            const char* names[4]; bool dirs[4]; uint8_t n = 0;
            for (uint8_t i = start; i < entryCount_ && n < 4; ++i, ++n) { names[n] = entries_[i].name; dirs[n] = entries_[i].isDir; }
            display_.drawBrowser(browserPath_, names, dirs, n, selected_ - start);
        }
    } else if (screen_ == Screen::FileActions) display_.showMenu(selectedFile_, fileActions, 3, selected_);
    else if (screen_ == Screen::DeleteConfirm) display_.showMenu("Delete file?", confirm, 2, selected_, selectedFile_);
}

void MenuSystem::enterBrowser(const char* path) {
    browserReturnScreen_ = screen_;
    strncpy(browserPath_, path, sizeof(browserPath_) - 1);
    browserPath_[sizeof(browserPath_) - 1] = '\0';
    screen_ = Screen::Browser;
    selected_ = 0;
    refreshBrowser();
    render();
}

void MenuSystem::refreshBrowser() { entryCount_ = sd_.listDir(browserPath_, entries_, Config::MAX_DIR_ENTRIES); }

void MenuSystem::joinPath(char* out, uint8_t outLen, const char* base, const char* name) {
    if (strcmp(base, "/") == 0) snprintf(out, outLen, "/%s", name);
    else snprintf(out, outLen, "%s/%s", base, name);
}

void MenuSystem::parentPath(char* path) {
    if (strcmp(path, "/") == 0) return;
    char* slash = strrchr(path, '/');
    if (!slash || slash == path) strcpy(path, "/");
    else *slash = '\0';
}

void MenuSystem::showTransient(const char* title, const char* a, const char* b) {
    returnScreen_ = screen_ == Screen::DeleteConfirm ? Screen::Browser : screen_;
    screen_ = Screen::Message;
    messageUntilMs_ = millis() + 900;
    display_.showMessage(title, a, b, "Please wait");
}
