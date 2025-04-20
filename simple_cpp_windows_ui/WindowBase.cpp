#include "pch.h"
#include "WindowBase.h"
#include <string>

WindowBase::WindowBase(HINSTANCE hInst, const std::wstring& className)
    : hInstance(hInst), wndClassName(className) {
    if (!Register()) {
        MessageBox(nullptr, L"Failed to register window class", L"Error", MB_ICONERROR);
    }
}

bool WindowBase::Register() {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowBase::WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = wndClassName.c_str();
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc)) {
        DWORD error = GetLastError();
        std::wstring msg = L"RegisterClass failed. Error: " + std::to_wstring(error);
        return false;
    }
    return true;
}

bool WindowBase::Create(int nCmdShow, int width, int height) {
    hWnd = CreateWindow(
        wndClassName.c_str(),
        GetWindowTitle(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr, nullptr, hInstance, this
    );

    if (!hWnd) {
        DWORD error = GetLastError();
        std::wstring msg = L"CreateWindow failed. Error: " + std::to_wstring(error);
        return false;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return true;
}

LRESULT CALLBACK WindowBase::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    WindowBase* pThis = nullptr;

    if (msg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = static_cast<WindowBase*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        pThis->hWnd = hWnd;
    }
    else {
        pThis = reinterpret_cast<WindowBase*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    return pThis ? pThis->HandleMessage(msg, wParam, lParam)
        : DefWindowProc(hWnd, msg, wParam, lParam);
}