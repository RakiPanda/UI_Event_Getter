#include <iostream>
#include <thread>
#include <windows.h>
#include "KeyboardHook.h"
#include "Utils.h"

int main() {
    if (!IsRunningAsAdmin()) {
        std::cerr << "This program must be run as an administrator." << std::endl;
        std::cout << "Press Enter to stop logging..." << std::endl;
        std::cin.get();
        return 1;
    }

    // KeyboardHookインスタンスを作成
    KeyboardHook keyboardHook;

    // フックを実行するスレッドを開始
    std::thread hookThread([&keyboardHook]() {
        if (!keyboardHook.Set()) {
            std::cerr << "Failed to set keyboard hook!" << std::endl;
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
    keyboardHook.Release();

    // スレッドを切り離し
    if (hookThread.joinable()) {
        hookThread.detach();
    }

    return 0;
}
