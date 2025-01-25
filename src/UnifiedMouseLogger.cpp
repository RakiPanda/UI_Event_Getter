#include "UnifiedMouseLogger.h"
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <sstream>

HHOOK UnifiedMouseLogger::hMouseHook = NULL;
std::ofstream UnifiedMouseLogger::logFile;
std::chrono::time_point<std::chrono::steady_clock> UnifiedMouseLogger::startTime;

UnifiedMouseLogger::UnifiedMouseLogger() {
    std::filesystem::create_directories("./UI_logs");

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");
    std::string filename = "./UI_logs/mouse_log_" + ss.str() + ".txt";

    logFile.open(filename);
    startTime = std::chrono::steady_clock::now();
}

UnifiedMouseLogger::~UnifiedMouseLogger() {
    Stop();
}

LRESULT CALLBACK UnifiedMouseLogger::MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
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

void UnifiedMouseLogger::Start() {
    hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);
    if (hMouseHook == NULL) {
        std::cerr << "Failed to install mouse hook!" << std::endl;
        return;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void UnifiedMouseLogger::Stop() {
    if (hMouseHook) {
        UnhookWindowsHookEx(hMouseHook);
        hMouseHook = NULL;
    }
    if (logFile.is_open()) {
        logFile.close();
    }
}
