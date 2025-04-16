
#include <windows.h>
#include "MainWindow.h"
#include "WindowBase.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    MainWindow mainWin(hInstance);
    return mainWin.Run(nCmdShow);
}