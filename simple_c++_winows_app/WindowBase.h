#pragma once
#include <windows.h>

class WindowBase {
public:
    WindowBase(HINSTANCE hInst, LPCWSTR className);
    virtual ~WindowBase() = default;

    HWND GetHwnd() const { return hWnd; }
    virtual LPCWSTR GetWindowTitle() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

protected:
    HWND hWnd;
    HINSTANCE hInstance;
    LPCWSTR wndClassName;

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void Register();
    void Create(int nCmdShow, int width = 400, int height = 300);
};