#include "MouseLogger.h"
#include <iostream>
#include <fstream>
#include <filesystem> // 追加
#include <chrono> // 追加
#include <iomanip> // 追加
#include <sstream> // 追加

// フックハンドルの初期化
HHOOK MouseLogger::hMouseHook = NULL;

// ログファイル
std::ofstream logFile;
std::chrono::time_point<std::chrono::steady_clock> startTime; // 追加

// コンストラクタ
MouseLogger::MouseLogger() {
    // ./UI_logsが存在しない場合はエラーが発生する
    std::filesystem::create_directories("./UI_logs"); // フォルダを生成

    // 現在の日時を取得してファイル名に追加
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");
    std::string filename = "./UI_logs/mouse_log_" + ss.str() + ".txt";

    logFile.open(filename);
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

        // 経過時間を取得
        auto now = std::chrono::steady_clock::now(); // 追加
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count(); // 変更

        // ログ内容を生成
        std::stringstream logStream;
        logStream << "Mouse Position: (" << x << ", " << y << "), Elapsed Time: " << elapsed << " ms";

        // 標準出力に座標を表示
        std::cout << logStream.str() << std::endl;

        // ログファイルに座標を記録
        if (logFile.is_open()) {
            logFile << logStream.str() << std::endl;
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
