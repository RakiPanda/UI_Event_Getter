#include "KeyboardHookDll.h"
#include <iostream>

HHOOK g_hHook = NULL;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

BOOL SetKeyboardHook() {
    g_hHook = ::SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, GetModuleHandle(NULL), 0);
    if (!g_hHook) {
        std::cerr << "Failed to install keyboard hook! Error: " << GetLastError() << std::endl;
        return FALSE;
    }
    return TRUE;
}

BOOL ReleaseKeyboardHook() {
    BOOL bRes = FALSE;
    if (g_hHook) {
        bRes = ::UnhookWindowsHookEx(g_hHook);
        g_hHook = NULL;
    }
    return bRes;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0 || nCode == HC_NOREMOVE) {
        return ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
    }

    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;
        std::cout << "Key Pressed: " << pKeyboard->vkCode << std::endl;
        // キーコードをインクリメント
        ++pKeyboard->vkCode;
        ::PostMessage(::GetFocus(), wParam, pKeyboard->vkCode, lParam);
    }

    return ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
}
