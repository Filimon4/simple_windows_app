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

typedef const wchar_t* (*ProcessFunc)(const wchar_t*);

struct AppCoreLib {
    HMODULE hModule = nullptr;
    ProcessFunc processArray = nullptr;
    ProcessFunc processMatrix = nullptr;

    bool Load() {
        hModule = LoadLibrary(L"simple_cpp_windows_core.dll");
        if (!hModule) {
            MessageBox(nullptr, L"Не удалось загрузить simple_cpp_windows_core.dll", L"Ошибка", MB_ICONERROR);
            return false;
        }

        processArray = reinterpret_cast<ProcessFunc>(GetProcAddress(hModule, "processArrayInt"));
        processMatrix = reinterpret_cast<ProcessFunc>(GetProcAddress(hModule, "processMatrixInt"));

        if (!processArray || !processMatrix) {
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
        static InputArrayWindow arrayWindow(hInstance);
        arrayWindow.ShowDialog(hWnd, 400, 300);
        std::wstring result = arrayWindow.GetResult();
        const wchar_t* processed = coreLib.processArray(result.c_str());
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
    default:
        break;
    }
}