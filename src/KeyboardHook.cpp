#include "KeyboardHook.h"
#include <iostream>

HHOOK KeyboardHook::m_hHook = NULL;

KeyboardHook::KeyboardHook() {
    // コンストラクタ
}

KeyboardHook::~KeyboardHook() {
    Release();
}

BOOL KeyboardHook::Set() {
    m_hHook = ::SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardHook::KeyboardProc, NULL, 0);
    if (!m_hHook) {
        std::cerr << "Failed to install keyboard hook! Error: " << GetLastError() << std::endl;
        return FALSE;
    }
    return TRUE;
}

BOOL KeyboardHook::Release() {
    BOOL bRes = FALSE;
    if (m_hHook) {
        bRes = ::UnhookWindowsHookEx(m_hHook);
        m_hHook = NULL;
    }
    return bRes;
}

LRESULT CALLBACK KeyboardHook::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0 || nCode == HC_NOREMOVE) {
        return ::CallNextHookEx(m_hHook, nCode, wParam, lParam);
    }

    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;
        std::cout << "Key Pressed: " << pKeyboard->vkCode << std::endl;
        // キーコードをインクリメント
        ++pKeyboard->vkCode;
        ::PostMessage(::GetFocus(), wParam, pKeyboard->vkCode, lParam);
    }

    return ::CallNextHookEx(m_hHook, nCode, wParam, lParam);
}
