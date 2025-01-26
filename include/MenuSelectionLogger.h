#ifndef MENU_SELECTION_LOGGER_H
#define MENU_SELECTION_LOGGER_H

#include <windows.h>
#include <fstream>
#include <chrono>

class MenuSelectionLogger {
public:
    MenuSelectionLogger();
    ~MenuSelectionLogger();
    void Start();
    void Stop();

private:
    static LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HHOOK hMenuHook;
    static std::ofstream logFile;
    static std::chrono::time_point<std::chrono::steady_clock> startTime;
};

#endif // MENU_SELECTION_LOGGER_H
