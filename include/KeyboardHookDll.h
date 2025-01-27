#ifndef KEYBOARD_HOOK_DLL_H
#define KEYBOARD_HOOK_DLL_H

#include <windows.h>

extern "C" __declspec(dllexport) BOOL SetKeyboardHook();
extern "C" __declspec(dllexport) BOOL ReleaseKeyboardHook();

#endif // KEYBOARD_HOOK_DLL_H
