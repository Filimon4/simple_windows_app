#include "ButtonLogic.h"
#include "MainWindow.h"
#include "InputArrayWindow.h"
#include "InputMatrixWindow.h"
#include <string>
#include "AppCore.h"

#define BTN1_ID 101
#define BTN2_ID 102

void HandleButtonClick(HWND hWnd, WPARAM wParam, HINSTANCE hInstance, MainWindow* pMain) {
    switch (LOWORD(wParam)) {
    case BTN1_ID: {
        static InputArrayWindow arrayWindow(hInstance);
        arrayWindow.ShowDialog(hWnd, 400, 300);
        std::wstring result = arrayWindow.GetResult();
        std::wstring swapResult = processArrayInt(result);
        SetWindowTextW(pMain->GetResultBoxHandle(), swapResult.c_str());
        break;
    }
    case BTN2_ID: {
        static InputMatrixWindow matrixWindow(hInstance);
        matrixWindow.ShowDialog(hWnd, 400, 300);
        std::wstring result = matrixWindow.GetResult();
        std::wstring swapResult = processMatrixInt(result);
        SetWindowTextW(pMain->GetResultBoxHandle(), swapResult.c_str());
        break;
    }
    default:
        break;
    }
}
