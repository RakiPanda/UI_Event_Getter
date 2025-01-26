#ifndef WINDOW_SELECTION_LOGGER_H
#define WINDOW_SELECTION_LOGGER_H

#include <windows.h>
#include <fstream>
#include <chrono>

class WindowSelectionLogger {
public:
    WindowSelectionLogger();
    ~WindowSelectionLogger();
    void Start();
    void Stop();

private:
    static LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HHOOK hWindowHook;
    static std::ofstream logFile;
    static std::chrono::time_point<std::chrono::steady_clock> startTime;
};

#endif // WINDOW_SELECTION_LOGGER_H
