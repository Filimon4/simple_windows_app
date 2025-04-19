#pragma once
#include <windows.h>
#include "WindowBase.h"


#ifdef APPCORE_EXPORTS
#define APPCORE_API __declspec(dllexport)
#else
#define APPCORE_API __declspec(dllimport)
#endif

class APPCORE_API InputDialogBase : public WindowBase {
public:
    InputDialogBase(HINSTANCE hInst, LPCWSTR className);
    virtual ~InputDialogBase() = default;

    void ShowDialog(HWND parent, int width = 300, int height = 200);
    virtual LPCWSTR GetWindowTitle() const = 0;
    virtual LRESULT DialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override {
        return DialogProc(GetHwnd(), uMsg, wParam, lParam);
    }

protected:
    static LRESULT CALLBACK StaticDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    bool isDialogActive = false;
    HWND hParent = nullptr;
};