#include "KeyboardLogger.h"
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <map>

HHOOK KeyboardLogger::hKeyboardHook = NULL;
std::ofstream KeyboardLogger::logFile;
std::chrono::time_point<std::chrono::steady_clock> KeyboardLogger::startTime;

std::map<int, std::string> keyMap = {
    {0x41, "A"}, {0x42, "B"}, {0x43, "C"}, {0x44, "D"}, {0x45, "E"},
    {0x46, "F"}, {0x47, "G"}, {0x48, "H"}, {0x49, "I"}, {0x4A, "J"},
    {0x4B, "K"}, {0x4C, "L"}, {0x4D, "M"}, {0x4E, "N"}, {0x4F, "O"},
    {0x50, "P"}, {0x51, "Q"}, {0x52, "R"}, {0x53, "S"}, {0x54, "T"},
    {0x55, "U"}, {0x56, "V"}, {0x57, "W"}, {0x58, "X"}, {0x59, "Y"},
    {0x5A, "Z"}, {0x30, "0"}, {0x31, "1"}, {0x32, "2"}, {0x33, "3"},
    {0x34, "4"}, {0x35, "5"}, {0x36, "6"}, {0x37, "7"}, {0x38, "8"},
    {0x39, "9"}, {VK_SPACE, "Space"}, {VK_RETURN, "Enter"}, {VK_BACK, "Backspace"},
    {VK_TAB, "Tab"}, {VK_SHIFT, "Shift"}, {VK_CONTROL, "Ctrl"}, {VK_MENU, "Alt"},
    {VK_CAPITAL, "Caps Lock"}, {VK_ESCAPE, "Esc"}, {VK_PRIOR, "Page Up"}, {VK_NEXT, "Page Down"},
    {VK_END, "End"}, {VK_HOME, "Home"}, {VK_LEFT, "Left Arrow"}, {VK_UP, "Up Arrow"},
    {VK_RIGHT, "Right Arrow"}, {VK_DOWN, "Down Arrow"}, {VK_INSERT, "Insert"}, {VK_DELETE, "Delete"},
    {VK_NUMPAD0, "Numpad 0"}, {VK_NUMPAD1, "Numpad 1"}, {VK_NUMPAD2, "Numpad 2"}, {VK_NUMPAD3, "Numpad 3"},
    {VK_NUMPAD4, "Numpad 4"}, {VK_NUMPAD5, "Numpad 5"}, {VK_NUMPAD6, "Numpad 6"}, {VK_NUMPAD7, "Numpad 7"},
    {VK_NUMPAD8, "Numpad 8"}, {VK_NUMPAD9, "Numpad 9"}, {VK_MULTIPLY, "Numpad *"}, {VK_ADD, "Numpad +"},
    {VK_SUBTRACT, "Numpad -"}, {VK_DECIMAL, "Numpad ."}, {VK_DIVIDE, "Numpad /"}, {VK_F1, "F1"},
    {VK_F2, "F2"}, {VK_F3, "F3"}, {VK_F4, "F4"}, {VK_F5, "F5"}, {VK_F6, "F6"},
    {VK_F7, "F7"}, {VK_F8, "F8"}, {VK_F9, "F9"}, {VK_F10, "F10"}, {VK_F11, "F11"},
    {VK_F12, "F12"}, {VK_NUMLOCK, "Num Lock"}, {VK_SCROLL, "Scroll Lock"},
    {VK_LSHIFT, "Left Shift"}, {VK_RSHIFT, "Right Shift"}, {VK_LCONTROL, "Left Ctrl"}, {VK_RCONTROL, "Right Ctrl"},
    {VK_LMENU, "Left Alt"}, {VK_RMENU, "Right Alt"}, {VK_LWIN, "Left Win"}, {VK_RWIN, "Right Win"},
    {VK_APPS, "Apps"}, {VK_SLEEP, "Sleep"}, {VK_VOLUME_MUTE, "Volume Mute"}, {VK_VOLUME_DOWN, "Volume Down"},
    {VK_VOLUME_UP, "Volume Up"}, {VK_MEDIA_NEXT_TRACK, "Next Track"}, {VK_MEDIA_PREV_TRACK, "Previous Track"},
    {VK_MEDIA_STOP, "Stop"}, {VK_MEDIA_PLAY_PAUSE, "Play/Pause"}, {VK_LAUNCH_MAIL, "Launch Mail"},
    {VK_LAUNCH_MEDIA_SELECT, "Launch Media Select"}, {VK_LAUNCH_APP1, "Launch App1"}, {VK_LAUNCH_APP2, "Launch App2"},
    {VK_OEM_1, "OEM 1"}, {VK_OEM_PLUS, "OEM Plus"}, {VK_OEM_COMMA, "OEM Comma"}, {VK_OEM_MINUS, "OEM Minus"},
    {VK_OEM_PERIOD, "OEM Period"}, {VK_OEM_2, "OEM 2"}, {VK_OEM_3, "OEM 3"}, {VK_OEM_4, "OEM 4"},
    {VK_OEM_5, "OEM 5"}, {VK_OEM_6, "OEM 6"}, {VK_OEM_7, "OEM 7"}, {VK_OEM_8, "OEM 8"},
    {VK_OEM_102, "OEM 102"}, {VK_PROCESSKEY, "Process Key"}, {VK_PACKET, "Packet"}, {VK_ATTN, "Attn"},
    {VK_CRSEL, "CrSel"}, {VK_EXSEL, "ExSel"}, {VK_EREOF, "Erase EOF"}, {VK_PLAY, "Play"},
    {VK_ZOOM, "Zoom"}, {VK_NONAME, "No Name"}, {VK_PA1, "PA1"}, {VK_OEM_CLEAR, "OEM Clear"}
};

KeyboardLogger::KeyboardLogger() {
    std::filesystem::create_directories("./UI_logs");

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");
    std::string filename = "./UI_logs/keyboard_log_" + ss.str() + ".txt";

    logFile.open(filename);
    startTime = std::chrono::steady_clock::now();
}

KeyboardLogger::~KeyboardLogger() {
    Stop();
}

LRESULT CALLBACK KeyboardLogger::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

        std::stringstream logStream;
        logStream << "Key Pressed: " << pKeyboard->vkCode;

        if (keyMap.find(pKeyboard->vkCode) != keyMap.end()) {
            logStream << " (" << keyMap[pKeyboard->vkCode] << ")";
        }

        logStream << ", Elapsed Time: " << elapsed << " ms";

        std::cout << logStream.str() << std::endl;

        if (logFile.is_open()) {
            logFile << logStream.str() << std::endl;
        }
    }
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

void KeyboardLogger::Start() {
    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hKeyboardHook == NULL) {
        std::cerr << "Failed to install keyboard hook!" << std::endl;
        return;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void KeyboardLogger::Stop() {
    if (hKeyboardHook) {
        UnhookWindowsHookEx(hKeyboardHook);
        hKeyboardHook = NULL;
    }
    if (logFile.is_open()) {
        logFile.close();
    }
}
