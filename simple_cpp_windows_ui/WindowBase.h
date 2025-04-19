#pragma once
#include <windows.h>
#include <string>

#ifdef APPCORE_EXPORTS
#define APPCORE_API __declspec(dllexport)
#else
#define APPCORE_API __declspec(dllimport)
#endif

class APPCORE_API WindowBase {
public:
    WindowBase(HINSTANCE hInst, const std::wstring& className);
    virtual ~WindowBase() = default;

    HWND GetHwnd() const { return hWnd; }
    virtual LPCWSTR GetWindowTitle() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

protected:
    HWND hWnd = nullptr;
    HINSTANCE hInstance = nullptr;
    std::wstring wndClassName;

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    bool Register();
    bool Create(int nCmdShow, int width = 400, int height = 300);
};