#include "ButtonLogic.h"
#include <windows.h>
#include "InputArrayWindow.h"
#include "InputMatrixWindow.h"

#define BTN1_ID 101
#define BTN2_ID 102

void HandleButtonClick(HWND hWnd, WPARAM wParam, HINSTANCE hInstance) {
    switch (LOWORD(wParam)) {
    case BTN1_ID: {
        static InputArrayWindow arrayWindow(hInstance);
        arrayWindow.ShowDialog(hWnd);
        break;
    }
    case BTN2_ID: {
        static InputMatrixWindow matrixWindow(hInstance);
        matrixWindow.ShowDialog(hWnd);
        break;
    }
    default:
        break;
    }
}