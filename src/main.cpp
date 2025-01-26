#include <iostream>
#include <thread>
#include <windows.h>
#include "UnifiedLoggers.h"
#include "Utils.h" // 追加

int main() {
    if (!IsRunningAsAdmin()) {
        std::cerr << "This program must be run as an administrator." << std::endl;
        return 1;
    }

    // UnifiedLoggersインスタンスを作成
    UnifiedLoggers logger;

    // フックを実行するスレッドを開始
    std::thread hookThread([&logger]() {
        logger.Start();
    });

    // ユーザーが終了するまでプログラムを実行
    std::cout << "Press Enter to stop logging..." << std::endl;
    std::cin.get();

    // フックを停止して終了
    logger.Stop();

    // スレッドを切り離し
    if (hookThread.joinable()) {
        hookThread.detach();
    }

    return 0;
}
