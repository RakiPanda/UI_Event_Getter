#ifndef KEYBOARD_HOOK_H
#define KEYBOARD_HOOK_H

#include <windows.h>

class KeyboardHook {
public:
    KeyboardHook();
    ~KeyboardHook();
    BOOL Set();
    BOOL Release();

private:
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HHOOK m_hHook;
};

#endif // KEYBOARD_HOOK_H
