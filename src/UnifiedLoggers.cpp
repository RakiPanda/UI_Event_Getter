#include "UnifiedLoggers.h"
#include "MouseMiddleClickLogger.h"
#include "KeyboardLogger.h"
#include "MenuSelectionLogger.h"
#include "WindowSelectionLogger.h" // 追加
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <map>

HHOOK UnifiedLoggers::hMouseHook = NULL;
HHOOK UnifiedLoggers::hKeyboardHook = NULL;
HHOOK UnifiedLoggers::hMenuHook = NULL;
HHOOK UnifiedLoggers::hWindowHook = NULL; // 追加
std::ofstream UnifiedLoggers::logFile;
std::chrono::time_point<std::chrono::steady_clock> UnifiedLoggers::startTime;

UnifiedLoggers::UnifiedLoggers() {
    std::filesystem::create_directories("./UI_logs");

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");
    std::string filename = "./UI_logs/event_log_" + ss.str() + ".txt";

    logFile.open(filename);
    startTime = std::chrono::steady_clock::now();
}

UnifiedLoggers::~UnifiedLoggers() {
    Stop();
}

LRESULT CALLBACK UnifiedLoggers::MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        MSLLHOOKSTRUCT* pMouse = (MSLLHOOKSTRUCT*)lParam;

        int x = pMouse->pt.x;
        int y = pMouse->pt.y;

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

        std::stringstream logStream;

        if (wParam == WM_MOUSEMOVE) {
            logStream << "Mouse Position: (" << x << ", " << y << "), Elapsed Time: " << elapsed << " ms";
        } else if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN) {
            std::string button = (wParam == WM_LBUTTONDOWN) ? "Left" : "Right";
            logStream << "Mouse Click: " << button << " Button at (" << x << ", " << y << "), Elapsed Time: " << elapsed << " ms";
        } else if (wParam == WM_MOUSEWHEEL) {
            int delta = GET_WHEEL_DELTA_WPARAM(pMouse->mouseData);
            logStream << "Mouse Wheel: Delta " << delta << " at (" << x << ", " << y << "), Elapsed Time: " << elapsed << " ms";
        } else if (wParam == WM_MBUTTONDOWN) {
            logStream << "Mouse Middle Click at (" << x << ", " << y << "), Elapsed Time: " << elapsed << " ms";
        }

        if (!logStream.str().empty()) {
            std::cout << logStream.str() << std::endl;
            if (logFile.is_open()) {
                logFile << logStream.str() << std::endl;
            }
        }
    }
    return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

LRESULT CALLBACK UnifiedLoggers::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
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

LRESULT CALLBACK UnifiedLoggers::MenuProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        CWPSTRUCT* pCwp = (CWPSTRUCT*)lParam;

        if (pCwp->message == WM_COMMAND) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

            std::stringstream logStream;
            logStream << "Menu Selection: Command ID " << LOWORD(pCwp->wParam) << ", Elapsed Time: " << elapsed << " ms";

            std::cout << logStream.str() << std::endl;

            if (logFile.is_open()) {
                logFile << logStream.str() << std::endl;
            }
        }
    }
    return CallNextHookEx(hMenuHook, nCode, wParam, lParam);
}

LRESULT CALLBACK UnifiedLoggers::WindowProc(int nCode, WPARAM wParam, LPARAM lParam) { // 追加
    if (nCode == HC_ACTION) {
        CWPSTRUCT* pCwp = (CWPSTRUCT*)lParam;

        if (pCwp->message == WM_SETFOCUS) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

            char windowTitle[256];
            GetWindowTextA(pCwp->hwnd, windowTitle, sizeof(windowTitle));

            std::stringstream logStream;
            logStream << "Window Selected: " << windowTitle << ", Elapsed Time: " << elapsed << " ms";

            std::cout << logStream.str() << std::endl;

            if (logFile.is_open()) {
                logFile << logStream.str() << std::endl;
            }
        }
    }
    return CallNextHookEx(hWindowHook, nCode, wParam, lParam);
}

void UnifiedLoggers::Start() {
    hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);
    if (hMouseHook == NULL) {
        std::cerr << "Failed to install mouse hook! Error: " << GetLastError() << std::endl;
        return;
    }

    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hKeyboardHook == NULL) {
        std::cerr << "Failed to install keyboard hook! Error: " << GetLastError() << std::endl;
        return;
    }

    hMenuHook = SetWindowsHookEx(WH_CALLWNDPROC, MenuProc, NULL, GetCurrentThreadId());
    if (hMenuHook == NULL) {
        std::cerr << "Failed to install menu hook! Error: " << GetLastError() << std::endl;
        return;
    }

    hWindowHook = SetWindowsHookEx(WH_CALLWNDPROC, WindowProc, NULL, GetCurrentThreadId()); // 追加
    if (hWindowHook == NULL) {
        std::cerr << "Failed to install window hook! Error: " << GetLastError() << std::endl;
        return;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void UnifiedLoggers::Stop() {
    if (hMouseHook) {
        UnhookWindowsHookEx(hMouseHook);
        hMouseHook = NULL;
    }
    if (hKeyboardHook) {
        UnhookWindowsHookEx(hKeyboardHook);
        hKeyboardHook = NULL;
    }
    if (hMenuHook) {
        UnhookWindowsHookEx(hMenuHook);
        hMenuHook = NULL;
    }
    if (hWindowHook) { // 追加
        UnhookWindowsHookEx(hWindowHook);
        hWindowHook = NULL;
    }
    if (logFile.is_open()) {
        logFile.close();
    }
}
