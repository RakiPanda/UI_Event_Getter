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
    static HHOOK hMouseHook;
    static std::ofstream logFile;
    static std::chrono::time_point<std::chrono::steady_clock> startTime;
};

#endif // UNIFIED_LOGGERS_H
