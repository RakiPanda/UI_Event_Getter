#ifndef MOUSE_CLICK_LOGGER_H
#define MOUSE_CLICK_LOGGER_H

#include <windows.h>
#include <fstream>
#include <chrono>

class MouseClickLogger {
public:
    MouseClickLogger();
    ~MouseClickLogger();
    void Start();
    void Stop();

private:
    static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HHOOK hMouseHook;
    static std::ofstream logFile;
    static std::chrono::time_point<std::chrono::steady_clock> startTime;
};

#endif // MOUSE_CLICK_LOGGER_H
