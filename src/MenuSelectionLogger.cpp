#include "MenuSelectionLogger.h"
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <sstream>

HHOOK MenuSelectionLogger::hMenuHook = NULL;
std::ofstream MenuSelectionLogger::logFile;
std::chrono::time_point<std::chrono::steady_clock> MenuSelectionLogger::startTime;

MenuSelectionLogger::MenuSelectionLogger() {
    std::filesystem::create_directories("./UI_logs");

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");
    std::string filename = "./UI_logs/menu_selection_log_" + ss.str() + ".txt";

    logFile.open(filename);
    startTime = std::chrono::steady_clock::now();
}

MenuSelectionLogger::~MenuSelectionLogger() {
    Stop();
}

LRESULT CALLBACK MenuSelectionLogger::CallWndProc(int nCode, WPARAM wParam, LPARAM lParam) {
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

void MenuSelectionLogger::Start() {
    hMenuHook = SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, NULL, GetCurrentThreadId());
    if (hMenuHook == NULL) {
        std::cerr << "Failed to install menu hook! Error: " << GetLastError() << std::endl;
        return;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void MenuSelectionLogger::Stop() {
    if (hMenuHook) {
        UnhookWindowsHookEx(hMenuHook);
        hMenuHook = NULL;
    }
    if (logFile.is_open()) {
        logFile.close();
    }
}
