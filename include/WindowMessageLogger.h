#ifndef WINDOW_MESSAGE_LOGGER_H
#define WINDOW_MESSAGE_LOGGER_H

#include <windows.h>
#include <fstream>
#include <chrono>

class WindowMessageLogger {
public:
    WindowMessageLogger();
    ~WindowMessageLogger();
    void Start();
    void Stop();

private:
    static LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HHOOK hCallWndProcHook;
    static std::ofstream logFile;
    static std::chrono::time_point<std::chrono::steady_clock> startTime;
};

#endif // WINDOW_MESSAGE_LOGGER_H
