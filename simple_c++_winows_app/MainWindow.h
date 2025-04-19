#pragma once
#include "WindowBase.h"

class MainWindow : public WindowBase {
public:
    MainWindow(HINSTANCE hInstance);
    int Run(int nCmdShow);
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;
    LPCWSTR GetWindowTitle() const override;

    HWND GetResultBoxHandle() const { return hResultBox; }

private:
    HWND hResultBox = nullptr;
};