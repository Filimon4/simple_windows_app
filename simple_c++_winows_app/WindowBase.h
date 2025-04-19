#pragma once
#include <windows.h>
#include <string>

class WindowBase {
public:
    WindowBase(HINSTANCE hInst, const std::wstring& className);
    virtual ~WindowBase() = default;

    HWND GetHwnd() const { return hWnd; }
    virtual LPCWSTR GetWindowTitle() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

protected:
    HWND hWnd = nullptr;
    HINSTANCE hInstance = nullptr;
    std::wstring wndClassName;  // Используем std::wstring вместо LPCWSTR

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    bool Register();  // Возвращает успешность регистрации
    bool Create(int nCmdShow, int width = 400, int height = 300);
};