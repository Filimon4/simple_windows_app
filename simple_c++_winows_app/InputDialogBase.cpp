#include "InputDialogBase.h"

InputDialogBase::InputDialogBase(HINSTANCE hInst, LPCWSTR className)
    : WindowBase(hInst, className), isDialogActive(false) {
}

void InputDialogBase::ShowDialog(HWND parent, int width, int height) {
    if (isDialogActive) {
        MessageBox(parent, L"Диалог уже открыт!", L"Ошибка", MB_ICONWARNING | MB_OK);
        if (hWnd) {
            SetForegroundWindow(hWnd);
            BringWindowToTop(hWnd);
        }
        return;
    }

    hParent = parent;
    isDialogActive = true;
    EnableWindow(parent, FALSE);

    hWnd = CreateWindowEx(
        WS_EX_DLGMODALFRAME,
        wndClassName.c_str(),
        GetWindowTitle(),
        WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        parent,
        nullptr,
        hInstance,
        this
    );

    if (!hWnd) {
        DWORD err = GetLastError();
        std::wstring errMsg = L"Ошибка создания окна. Код: " + std::to_wstring(err);
        MessageBox(parent, errMsg.c_str(), L"Ошибка", MB_ICONERROR);
        EnableWindow(parent, TRUE);
        isDialogActive = false;
        return;
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    MSG msg;
    while (IsWindow(hWnd)) {
        if (GetMessage(&msg, nullptr, 0, 0)) {
            if (!IsDialogMessage(hWnd, &msg)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else {
            break;
        }
    }

    EnableWindow(parent, TRUE);
    SetForegroundWindow(parent);
    SetActiveWindow(parent);
    BringWindowToTop(parent);
    isDialogActive = false;
}

LRESULT CALLBACK InputDialogBase::StaticDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    InputDialogBase* pThis = nullptr;

    if (msg == WM_NCCREATE) {
        pThis = static_cast<InputDialogBase*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else {
        pThis = reinterpret_cast<InputDialogBase*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis) {
        if (msg == WM_CLOSE) {
            DestroyWindow(hWnd);
            PostQuitMessage(0);
            return 0;
        }

        LRESULT result = pThis->DialogProc(hWnd, msg, wParam, lParam);

        if (msg == WM_DESTROY) {
            pThis->isDialogActive = false;
            if (pThis->hParent) {
                EnableWindow(pThis->hParent, TRUE);
                SetForegroundWindow(pThis->hParent);
            }
            pThis->hWnd = nullptr;
            return 0;
        }

        return result;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}