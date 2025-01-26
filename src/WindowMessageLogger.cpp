#include "WindowMessageLogger.h"
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <sstream>

HHOOK WindowMessageLogger::hCallWndProcHook = NULL;
std::ofstream WindowMessageLogger::logFile;
std::chrono::time_point<std::chrono::steady_clock> WindowMessageLogger::startTime;

WindowMessageLogger::WindowMessageLogger() {
    std::filesystem::create_directories("./UI_logs");

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");
    std::string filename = "./UI_logs/window_message_log_" + ss.str() + ".txt";

    logFile.open(filename);
    startTime = std::chrono::steady_clock::now();
}

WindowMessageLogger::~WindowMessageLogger() {
    Stop();
}

LRESULT CALLBACK WindowMessageLogger::CallWndProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        CWPSTRUCT* pCwp = (CWPSTRUCT*)lParam;

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

        std::stringstream logStream;
        logStream << "Window Message: " << pCwp->message << ", HWND: " << pCwp->hwnd << ", Elapsed Time: " << elapsed << " ms";

        std::cout << logStream.str() << std::endl;

        if (logFile.is_open()) {
            logFile << logStream.str() << std::endl;
        }
    }
    return CallNextHookEx(hCallWndProcHook, nCode, wParam, lParam);
}

void WindowMessageLogger::Start() {
    hCallWndProcHook = SetWindowsHookExA(WH_CALLWNDPROC, CallWndProc, NULL, 0);
    if (hCallWndProcHook == NULL) {
        std::cerr << "Failed to install CallWndProc hook! Error: " << GetLastError() << std::endl;
        return;
    }
    else{
        std::cout << "Window Message Logger is running..." << std::endl;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        std::cout << "Message received: " << msg.message << std::endl; // デバッグ用出力
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void WindowMessageLogger::Stop() {
    if (hCallWndProcHook) {
        UnhookWindowsHookEx(hCallWndProcHook);
        hCallWndProcHook = NULL;
    }
    if (logFile.is_open()) {
        logFile.close();
    }
}
