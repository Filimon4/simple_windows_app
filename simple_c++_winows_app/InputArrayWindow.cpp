#include "InputArrayWindow.h"

InputArrayWindow::InputArrayWindow(HINSTANCE hInst)
    : InputDialogBase(hInst, L"InputArrayWindowClass") {
}

LPCWSTR InputArrayWindow::GetWindowTitle() const {
    return L"���� �������";
}

LRESULT InputArrayWindow::DialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        return 0;
    case WM_DESTROY:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}