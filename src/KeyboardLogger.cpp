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
    {0x5A, "Z"}, {VK_SPACE, "Space"}, {VK_RETURN, "Enter"}, {VK_BACK, "Backspace"},
    {VK_TAB, "Tab"}, {VK_SHIFT, "Shift"}, {VK_CONTROL, "Ctrl"}, {VK_MENU, "Alt"}
    // 必要に応じて他のキーも追加
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
