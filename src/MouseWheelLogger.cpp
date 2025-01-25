#include "MouseWheelLogger.h"
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <sstream>

HHOOK MouseWheelLogger::hMouseHook = NULL;
std::ofstream MouseWheelLogger::logFile;
std::chrono::time_point<std::chrono::steady_clock> MouseWheelLogger::startTime;

MouseWheelLogger::MouseWheelLogger() {
    std::filesystem::create_directories("./UI_logs");

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");
    std::string filename = "./UI_logs/mouse_wheel_log_" + ss.str() + ".txt";

    logFile.open(filename);
    startTime = std::chrono::steady_clock::now();
}

MouseWheelLogger::~MouseWheelLogger() {
    Stop();
}

LRESULT CALLBACK MouseWheelLogger::MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && wParam == WM_MOUSEWHEEL) {
        MSLLHOOKSTRUCT* pMouse = (MSLLHOOKSTRUCT*)lParam;

        int x = pMouse->pt.x;
        int y = pMouse->pt.y;
        int delta = GET_WHEEL_DELTA_WPARAM(pMouse->mouseData);

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

        std::stringstream logStream;
        logStream << "Mouse Wheel: Delta " << delta << " at (" << x << ", " << y << "), Elapsed Time: " << elapsed << " ms";

        std::cout << logStream.str() << std::endl;

        if (logFile.is_open()) {
            logFile << logStream.str() << std::endl;
        }
    }
    return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

void MouseWheelLogger::Start() {
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

void MouseWheelLogger::Stop() {
    if (hMouseHook) {
        UnhookWindowsHookEx(hMouseHook);
        hMouseHook = NULL;
    }
    if (logFile.is_open()) {
        logFile.close();
    }
}
