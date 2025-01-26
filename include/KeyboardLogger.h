#ifndef KEYBOARD_LOGGER_H
#define KEYBOARD_LOGGER_H

#include <windows.h>
#include <fstream>
#include <chrono>

class KeyboardLogger {
public:
    KeyboardLogger();
    ~KeyboardLogger();
    void Start();
    void Stop();

private:
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HHOOK hKeyboardHook;
    static std::ofstream logFile;
    static std::chrono::time_point<std::chrono::steady_clock> startTime;
};

#endif // KEYBOARD_LOGGER_H
