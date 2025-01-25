#pragma once

#include <windows.h>

class MouseLogger {
public:
    // コンストラクタとデストラクタ
    MouseLogger();
    ~MouseLogger();

    // マウスフックを開始
    void Start();

    // マウスフックを停止
    void Stop();

private:
    // フックハンドル
    static HHOOK hMouseHook;

    // フックプロシージャ
    static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
};

