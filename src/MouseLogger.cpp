#include "MouseLogger.h"
#include <iostream>
#include <fstream>

// フックハンドルの初期化
HHOOK MouseLogger::hMouseHook = NULL;

// ログファイル
std::ofstream logFile("mouse_coordinates_log.txt");

// コンストラクタ
MouseLogger::MouseLogger() {}

// デストラクタ
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
            logFile << "Mouse Position: (" << x << ", " << y << ")" << std::endl;
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
