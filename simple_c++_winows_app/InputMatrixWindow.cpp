#include "InputMatrixWindow.h"

#define ID_BTN_ADD_ROW   2001
#define ID_BTN_PROCESS   2002
#define BASE_EDIT_ID     2100

InputMatrixWindow::InputMatrixWindow(HINSTANCE hInst)
    : InputDialogBase(hInst, L"InputMatrixWindowClass") {
}

LPCWSTR InputMatrixWindow::GetWindowTitle() const {
    return L"¬вод матрицы";
}

void InputMatrixWindow::AddEditBox(HWND hWnd, int yOffset) {
    HWND hEdit = CreateWindowEx(
        0, L"EDIT", nullptr,
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        20, yOffset, 250, 25,
        hWnd, (HMENU)(BASE_EDIT_ID + editBoxes.size()),
        GetModuleHandle(nullptr), nullptr
    );
    editBoxes.push_back(hEdit);
}

LRESULT InputMatrixWindow::DialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        AddEditBox(hWnd, 20); 

        CreateWindowEx(
            0, L"BUTTON", L"+",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            280, 20, 30, 25,
            hWnd, (HMENU)ID_BTN_ADD_ROW,
            GetModuleHandle(nullptr), nullptr
        );

        CreateWindowEx(
            0, L"BUTTON", L"ќбработать",
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            20, 60, 100, 30,
            hWnd, (HMENU)ID_BTN_PROCESS,
            GetModuleHandle(nullptr), nullptr
        );
        return 0;
    }
    case WM_SIZE: {
        RECT rc;
        GetClientRect(hWnd, &rc);

        int btnWidth = 100;
        int btnHeight = 30;
        int spacing = 10;

        int totalWidth = btnWidth * 2 + spacing;

        int startX = (rc.right - totalWidth) / 2;
        int y = rc.bottom - btnHeight - 20;

        HWND hProcess = GetDlgItem(hWnd, ID_BTN_PROCESS);
        HWND hAddRow = GetDlgItem(hWnd, ID_BTN_ADD_ROW);

        if (hProcess)
            SetWindowPos(hProcess, nullptr, startX, y, btnWidth, btnHeight, SWP_NOZORDER);
        if (hAddRow)
            SetWindowPos(hAddRow, nullptr, startX + btnWidth + spacing, y, btnWidth, btnHeight, SWP_NOZORDER);

        return 0;
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case ID_BTN_ADD_ROW: {
            int yOffset = 20 + static_cast<int>(editBoxes.size()) * 30;
            AddEditBox(hWnd, yOffset);
            return 0;
        }
        case ID_BTN_PROCESS: {
            std::wstring resultText;
            for (HWND hEdit : editBoxes) {
                wchar_t buffer[256];
                GetWindowText(hEdit, buffer, 256);
                resultText += buffer;
                resultText += L"\r\n";
            }

            result = resultText;
            DestroyWindow(hWnd);
            return 0;
        }
        }
        return 0;
    }

    case WM_CLOSE:
        DestroyWindow(hWnd);
        return 0;

    case WM_DESTROY:
        editBoxes.clear();
        return 0;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}
