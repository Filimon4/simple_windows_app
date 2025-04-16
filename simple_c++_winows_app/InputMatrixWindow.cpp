#include "InputMatrixWindow.h"

InputMatrixWindow::InputMatrixWindow(HINSTANCE hInst)
    : InputDialogBase(hInst, L"InputMatrixWindowClass") {
}

LPCWSTR InputMatrixWindow::GetWindowTitle() const {
    return L"¬вод матрицы";
}

LRESULT InputMatrixWindow::DialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        return 0;
    case WM_DESTROY:
        return 0;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}