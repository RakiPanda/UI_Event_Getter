#include <iostream>
#include <thread>
#include "MouseLogger.h"

int main() {
    // MouseLoggerインスタンスを作成
    MouseLogger logger;

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
