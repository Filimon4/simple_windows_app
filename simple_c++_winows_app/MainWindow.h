#pragma once
#include "WindowBase.h"

class MainWindow : public WindowBase {
public:
    bool IsByteModeEnabled() const { return byteModeEnabled; }
    MainWindow(HINSTANCE hInstance);
    int Run(int nCmdShow);
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;
    LPCWSTR GetWindowTitle() const override;

    HWND GetResultBoxHandle() const { return hResultBox; }
    bool byteModeEnabled = false;
    HWND hByteModeCheckbox = nullptr;

private:
    HWND hResultBox = nullptr;
};