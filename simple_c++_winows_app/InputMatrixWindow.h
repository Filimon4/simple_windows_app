#pragma once
#include "InputDialogBase.h"
#include <string>
#include <vector>

class InputMatrixWindow : public InputDialogBase {
public:
    InputMatrixWindow(HINSTANCE hInst);
    LPCWSTR GetWindowTitle() const override;
    LRESULT DialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

    std::wstring GetResult() const { return result; }

private:
    std::vector<HWND> editBoxes;
    std::wstring result;
    void AddEditBox(HWND hWnd, int yOffset);
};