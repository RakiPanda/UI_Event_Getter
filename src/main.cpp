#include <iostream>
#include <thread>
#include <windows.h>
#include "Utils.h"

HOOKPROC hkprcSysMsg;
HHOOK hhookSysMsg = NULL;
HINSTANCE hinstDLL = NULL;

LRESULT CALLBACK SysMessageProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        MSG* msg = (MSG*)lParam;
        std::cout << "SysMessage: " << msg->message << " wParam: " << msg->wParam << " lParam: " << msg->lParam << std::endl;
    }
    return CallNextHookEx(hhookSysMsg, nCode, wParam, lParam);
}

int main() {
    if (!IsRunningAsAdmin()) {
        std::cerr << "This program must be run as an administrator." << std::endl;
        std::cout << "Press Enter to stop logging..." << std::endl;
        std::cin.get();
        return 1;
    }

    // フックを実行するスレッドを開始
    std::thread hookThread([]() {
        hinstDLL = LoadLibrary(TEXT("C:\\Windows\\System32\\user32.dll")); 
        hkprcSysMsg = (HOOKPROC)GetProcAddress(hinstDLL, "SysMessageProc");
        hhookSysMsg = SetWindowsHookEx( 
                    WH_SYSMSGFILTER,
                    hkprcSysMsg,
                    hinstDLL,
                    0);
        if (!hhookSysMsg) {
            std::cerr << "Failed to install sysmsg hook! Error: " << GetLastError() << std::endl;
            return;
        }

        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    });

    // ユーザーが終了するまでプログラムを実行
    std::cout << "Press Enter to stop logging..." << std::endl;
    std::cin.get();

    // フックを停止して終了
    if (hhookSysMsg) {
        UnhookWindowsHookEx(hhookSysMsg);
        hhookSysMsg = NULL;
    }

    // スレッドを切り離し
    if (hookThread.joinable()) {
        hookThread.detach();
    }

    return 0;
}
