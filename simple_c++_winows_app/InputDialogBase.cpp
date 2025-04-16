#include "InputDialogBase.h"

InputDialogBase::InputDialogBase(HINSTANCE hInst, LPCWSTR className)
    : WindowBase(hInst, className), isDialogActive(false) {
}

//InputDialogBase::~InputDialogBase() {
//    if (hWnd && IsWindow(hWnd)) {
//        DestroyWindow(hWnd);
//    }
//}

void InputDialogBase::ShowDialog(HWND parent, int width, int height) {
    if (isDialogActive) {
        MessageBox(parent, L"Диалоговое окно уже открыто!", L"Ошибка", MB_ICONWARNING | MB_OK);
        if (hWnd) {
            SetForegroundWindow(hWnd);
            BringWindowToTop(hWnd);
        }
        return;
    }

    hParent = parent;
    hWnd = CreateWindowEx(
        WS_EX_DLGMODALFRAME,
        wndClassName,
        GetWindowTitle(),
        WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        parent,
        nullptr,
        hInstance,
        this
    );

    if (hWnd) {
        isDialogActive = true;
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);
        EnableWindow(parent, FALSE);

        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0) && isDialogActive) {
            if (!IsDialogMessage(hWnd, &msg)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
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
        }

        return result;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}