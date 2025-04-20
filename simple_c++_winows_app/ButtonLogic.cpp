#include "ButtonLogic.h"
#include "MainWindow.h"
#include "InputArrayWindow.h"
#include "InputMatrixWindow.h"
#include <string>
#include "AppCore.h"
#include <stdexcept>
#include <windows.h>

#define BTN1_ID 101
#define BTN2_ID 102
#define CHK_BYTE_MODE_ID 103

typedef const wchar_t* (*CoreFunc)(const wchar_t*);

struct AppCoreLib {
    HMODULE hModule = nullptr;
    CoreFunc processArray = nullptr;
    CoreFunc processMatrix = nullptr;
    CoreFunc processMatrixByte = nullptr;
    CoreFunc processArrayByte = nullptr;

    bool Load() {
        hModule = LoadLibrary(L"simple_cpp_windows_core.dll");
        if (!hModule) {
            MessageBox(nullptr, L"Не удалось загрузить simple_cpp_windows_core.dll", L"Ошибка", MB_ICONERROR);
            return false;
        }

        processArray = reinterpret_cast<CoreFunc>(GetProcAddress(hModule, "processArrayInt"));
        processMatrix = reinterpret_cast<CoreFunc>(GetProcAddress(hModule, "processMatrixInt"));
        processMatrixByte = reinterpret_cast<CoreFunc>(GetProcAddress(hModule, "processMatrixByte"));
        processArrayByte = reinterpret_cast<CoreFunc>(GetProcAddress(hModule, "processArrayByte"));

        if (!processArray || !processMatrix || !processArrayByte || !processMatrixByte) {
            MessageBox(nullptr, L"Функции не найдены в DLL", L"Ошибка", MB_ICONERROR);
            FreeLibrary(hModule);
            hModule = nullptr;
            return false;
        }

        return true;
    }

    ~AppCoreLib() {
        if (hModule) {
            FreeLibrary(hModule);
        }
    }
};


void HandleButtonClick(HWND hWnd, WPARAM wParam, HINSTANCE hInstance, MainWindow* pMain) {
    static AppCoreLib coreLib;
    static bool isLoaded = coreLib.Load();

    if (!isLoaded) return;

    switch (LOWORD(wParam)) {
    case BTN1_ID: {
        static InputDialogBase* arrayWindow = new InputArrayWindow(hInstance);
        arrayWindow->ShowDialog(hWnd, 400, 300);
        std::wstring result = static_cast<InputArrayWindow*>(arrayWindow)->GetResult();
        const wchar_t* processed = nullptr;
        if (pMain->IsByteModeEnabled() == false) {
            processed = coreLib.processArray(result.c_str());
        }
        else {
            processed = coreLib.processArrayByte(result.c_str());
        }
        SetWindowTextW(pMain->GetResultBoxHandle(), processed);
        break;
    }
    case BTN2_ID: {
        static InputMatrixWindow matrixWindow(hInstance);
        matrixWindow.ShowDialog(hWnd, 400, 300);
        std::wstring result = matrixWindow.GetResult();
        const wchar_t* processed = coreLib.processMatrix(result.c_str());
        SetWindowTextW(pMain->GetResultBoxHandle(), processed);
        break;
    }
    case CHK_BYTE_MODE_ID:
        pMain->byteModeEnabled = (SendMessage(pMain->hByteModeCheckbox, BM_GETCHECK, 0, 0) == BST_CHECKED);
        break;
    default:
        break;
    }
}