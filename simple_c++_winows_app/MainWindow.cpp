#include "MainWindow.h"
#include "ButtonLogic.h"
#include "resource.h"

#define BTN1_ID 101
#define BTN2_ID 102

MainWindow::MainWindow(HINSTANCE hInstance)
    : WindowBase(hInstance, L"MainWindowClass") {
}

int MainWindow::Run(int nCmdShow) {
    Create(nCmdShow, 500, 500);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT MainWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        CreateWindow(L"BUTTON", L"Открыть Диалог 1", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            50, 50, 150, 30, hWnd, (HMENU)BTN1_ID, hInstance, nullptr);
        CreateWindow(L"BUTTON", L"Открыть Диалог 2", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            50, 100, 150, 30, hWnd, (HMENU)BTN2_ID, hInstance, nullptr);
        break;

    case WM_COMMAND:
        HandleButtonClick(hWnd, wParam, this->hInstance);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}