#include <iostream>
#include <thread>
#include <windows.h>
#include "UnifiedLoggers.h"

bool IsRunningAsAdmin() {
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;

    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 
                                 0, 0, 0, 0, 0, 0, &adminGroup)) {
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }

    return isAdmin;
}

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
