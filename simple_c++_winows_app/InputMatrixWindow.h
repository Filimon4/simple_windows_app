#pragma once
#include "InputDialogBase.h"

class InputMatrixWindow : public InputDialogBase {
public:
    InputMatrixWindow(HINSTANCE hInst);
    LPCWSTR GetWindowTitle() const override;
    LRESULT DialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
};
