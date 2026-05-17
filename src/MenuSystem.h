#pragma once
#include "BuzzerManager.h"
#include "DisplayManager.h"
#include "InputMonitor.h"
#include "PlatformCodec.h"
#include "SdManager.h"

class MenuSystem {
public:
    MenuSystem(DisplayManager& display, SdManager& sd, BuzzerManager& buzzer, InputMonitor& monitor,
               PlatformCodec** codecs, uint8_t codecCount);
    void begin();
    void update();
private:
    enum class Screen { Main, Platform, Tools, Settings, Monitoring, Browser, FileActions, DeleteConfirm, Message };
    enum class Button { None, Up, Down, Select, Back };

    Button readButton();
    void render();
    void move(int delta, uint8_t count);
    void select();
    void back();
    void enterBrowser(const char* path);
    void refreshBrowser();
    void joinPath(char* out, uint8_t outLen, const char* base, const char* name);
    void parentPath(char* path);
    void showTransient(const char* title, const char* a, const char* b = nullptr);

    DisplayManager& display_;
    SdManager& sd_;
    BuzzerManager& buzzer_;
    InputMonitor& monitor_;
    PlatformCodec** codecs_;
    uint8_t codecCount_;
    Screen screen_ = Screen::Main;
    Screen returnScreen_ = Screen::Main;
    Screen browserReturnScreen_ = Screen::Main;
    uint8_t selected_ = 0;
    uint8_t activeCodec_ = 0;
    uint32_t lastButtonMs_ = 0;
    uint32_t lastMonitorMs_ = 0;
    uint32_t messageUntilMs_ = 0;
    char browserPath_[Config::MAX_PATH_LEN] = "/";
    DirEntry entries_[Config::MAX_DIR_ENTRIES];
    uint8_t entryCount_ = 0;
    char selectedFile_[Config::MAX_PATH_LEN] = {0};
};
