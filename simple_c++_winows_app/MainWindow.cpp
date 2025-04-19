#include "MainWindow.h"
#include "ButtonLogic.h"
#include <windows.h>
#include <string>

#define BTN1_ID 101
#define BTN2_ID 102

MainWindow::MainWindow(HINSTANCE hInstance)
    : WindowBase(hInstance, L"MainWindowClass") {
}

int MainWindow::Run(int nCmdShow) {
    Create(nCmdShow, 700, 500);
    if (!hWnd) {
        MessageBox(nullptr, L"Окно не создано!", L"Ошибка", MB_ICONERROR);
        return -1;
    }
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LPCWSTR MainWindow::GetWindowTitle() const {
    return L"Главное окно";
}

LRESULT MainWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        CreateWindow(
            L"BUTTON", L"Открыть Диалог 1", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            50, 50, 150, 30, hWnd, (HMENU)BTN1_ID, hInstance, nullptr
        );
        CreateWindow(
            L"BUTTON", L"Открыть Диалог 2", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            50, 100, 150, 30, hWnd, (HMENU)BTN2_ID, hInstance, nullptr
        );

        hResultBox = CreateWindowW(
            L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_LEFT | ES_READONLY,
            250, 50, 420, 300, hWnd, nullptr, hInstance, nullptr
        );
        break;
    case WM_COMMAND:
        HandleButtonClick(hWnd, wParam, this->hInstance, this);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
