#pragma once
#include "WindowBase.h"

class MainWindow : public WindowBase {
public:
    MainWindow(HINSTANCE hInstance);
    int Run(int nCmdShow);

    LPCWSTR GetWindowTitle() const override { return L"Главное окно"; }
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;
};