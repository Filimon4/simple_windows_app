#include "InputArrayWindow.h"

#define ID_EDIT_INPUT 1001
#define ID_BTN_PROCESS 1002

InputArrayWindow::InputArrayWindow(HINSTANCE hInst)
    : InputDialogBase(hInst, L"InputArrayWindowClass") {
}

LPCWSTR InputArrayWindow::GetWindowTitle() const {
    return L"¬вод массива";
}

LRESULT InputArrayWindow::DialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindowEx(
            0,
            L"EDIT",
            nullptr,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            20, 20, 200, 25,
            hWnd,
            (HMENU)ID_EDIT_INPUT,
            GetModuleHandle(nullptr),
            nullptr
        );
        CreateWindowEx(
            0,
            L"BUTTON",
            L"ќбработать",
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            20, 60, 100, 30,
            hWnd,
            (HMENU)ID_BTN_PROCESS,
            GetModuleHandle(nullptr),
            nullptr
        );
        return 0;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == ID_BTN_PROCESS) {
            wchar_t buffer[256];
            GetDlgItemText(hWnd, ID_EDIT_INPUT, buffer, 256);

            std::wstring input(buffer);
            std::string inputStr(input.begin(), input.end());

            result = input;

            DestroyWindow(hWnd);
            return 0;
        }
        return 0;
    }
    case WM_SIZE: {
        RECT rc;
        GetClientRect(hWnd, &rc);

        int btnWidth = 100;
        int btnHeight = 30;

        int totalWidth = btnWidth * 2;

        int startX = (rc.right - totalWidth);
        int y = rc.bottom - btnHeight - 20;

        HWND hProcess = GetDlgItem(hWnd, ID_BTN_PROCESS);

        if (hProcess)
            SetWindowPos(hProcess, nullptr, startX, y, btnWidth, btnHeight, SWP_NOZORDER);

        return 0;
    }
    case WM_CLOSE:
        DestroyWindow(hWnd);
        return 0;

    case WM_DESTROY:
        return 0;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}