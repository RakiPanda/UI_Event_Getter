#include "WindowSelectionLogger.h"
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <sstream>

HHOOK WindowSelectionLogger::hWindowHook = NULL;
std::ofstream WindowSelectionLogger::logFile;
std::chrono::time_point<std::chrono::steady_clock> WindowSelectionLogger::startTime;

WindowSelectionLogger::WindowSelectionLogger() {
    std::filesystem::create_directories("./UI_logs");

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");
    std::string filename = "./UI_logs/window_selection_log_" + ss.str() + ".txt";

    logFile.open(filename);
    startTime = std::chrono::steady_clock::now();
}

WindowSelectionLogger::~WindowSelectionLogger() {
    Stop();
}

LRESULT CALLBACK WindowSelectionLogger::CallWndProc(int nCode, WPARAM wParam, LPARAM lParam) {
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

void WindowSelectionLogger::Start() {
    hWindowHook = SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, NULL, GetCurrentThreadId());
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

void WindowSelectionLogger::Stop() {
    if (hWindowHook) {
        UnhookWindowsHookEx(hWindowHook);
        hWindowHook = NULL;
    }
    if (logFile.is_open()) {
        logFile.close();
    }
}
