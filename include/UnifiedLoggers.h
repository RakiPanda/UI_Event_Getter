#ifndef UNIFIED_LOGGERS_H
#define UNIFIED_LOGGERS_H

#include <windows.h>
#include <fstream>
#include <chrono>

class UnifiedLoggers {
public:
    UnifiedLoggers();
    ~UnifiedLoggers();
    void Start();
    void Stop();

private:
    static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK MenuProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK WindowProc(int nCode, WPARAM wParam, LPARAM lParam); // 追加
    static HHOOK hMouseHook;
    static HHOOK hKeyboardHook;
    static HHOOK hMenuHook;
    static HHOOK hWindowHook; // 追加
    static std::ofstream logFile;
    static std::chrono::time_point<std::chrono::steady_clock> startTime;
};

#endif // UNIFIED_LOGGERS_H
