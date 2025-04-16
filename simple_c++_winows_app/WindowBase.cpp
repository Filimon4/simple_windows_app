#include "WindowBase.h"

WindowBase::WindowBase(HINSTANCE hInst, LPCWSTR className)
    : hInstance(hInst), wndClassName(className), hWnd(nullptr) {
    Register();
}

void WindowBase::Register() {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = wndClassName;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);
}

void WindowBase::Create(int nCmdShow, int width, int height) {
    hWnd = CreateWindow(wndClassName, GetWindowTitle(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, hInstance, this);

    if (hWnd) {
        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);
    }
}

LRESULT CALLBACK WindowBase::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    WindowBase* pThis = nullptr;

    if (msg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = static_cast<WindowBase*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else {
        pThis = reinterpret_cast<WindowBase*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis) {
        return pThis->HandleMessage(msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
