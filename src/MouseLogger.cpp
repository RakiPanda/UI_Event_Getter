#include "MouseLogger.h"
#include <iostream>
#include <fstream>
#include <filesystem> // 追加
#include <chrono> // 追加

// フックハンドルの初期化
HHOOK MouseLogger::hMouseHook = NULL;

// ログファイル
// ./UI_logsにtxtファイルを作成

std::ofstream logFile;
std::chrono::time_point<std::chrono::steady_clock> startTime; // 追加

// コンストラクタ
MouseLogger::MouseLogger() {
    // ./UI_logsが存在しない場合はエラーが発生する
    std::filesystem::create_directories("./UI_logs"); // フォルダを生成
    logFile.open("./UI_logs/mouse_log.txt");
    startTime = std::chrono::steady_clock::now(); // 追加
}

MouseLogger::~MouseLogger() {
    Stop();
}

// フックプロシージャ
LRESULT CALLBACK MouseLogger::MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && wParam == WM_MOUSEMOVE) {
        MSLLHOOKSTRUCT* pMouse = (MSLLHOOKSTRUCT*)lParam;

        // マウス座標を取得
        int x = pMouse->pt.x;
        int y = pMouse->pt.y;

        // 標準出力に座標を表示
        std::cout << "Mouse Position: (" << x << ", " << y << ")" << std::endl;

        // ログファイルに座標を記録
        if (logFile.is_open()) {
            auto now = std::chrono::steady_clock::now(); // 追加
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count(); // 追加
            logFile << "Mouse Position: (" << x << ", " << y << "), Elapsed Time: " << elapsed << " seconds" << std::endl; // 変更
        }
    }
    return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

// マウスフックを開始
void MouseLogger::Start() {
    hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);
    if (hMouseHook == NULL) {
        std::cerr << "Failed to install mouse hook!" << std::endl;
        return;
    }

    // メッセージループ
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// マウスフックを停止
void MouseLogger::Stop() {
    if (hMouseHook) {
        UnhookWindowsHookEx(hMouseHook);
        hMouseHook = NULL;
    }
    if (logFile.is_open()) {
        logFile.close();
    }
}
