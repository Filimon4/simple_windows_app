#pragma once
#include "InputDialogBase.h"

class InputArrayWindow : public InputDialogBase {
public:
    InputArrayWindow(HINSTANCE hInst);
    LPCWSTR GetWindowTitle() const override;
    LRESULT DialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
};