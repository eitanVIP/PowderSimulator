#include <windows.h>
#include <stdio.h>
#include "PowderSimulator.h"
#include "Window.h"
#include <math.h>

double time() {
    FILETIME ft;
    GetSystemTimePreciseAsFileTime(&ft); // requires Windows 8 or later
    ULARGE_INTEGER uli;
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;

    // FILETIME is in 100-ns intervals since Jan 1, 1601
    // Convert to seconds since 1970
    return (uli.QuadPart - 116444736000000000ULL) / 10000000.0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HWND hwnd;
    if (window_setupWindow(hInstance, nCmdShow, &hwnd) == 1)
        return 1;

    simulator_start(200, 200);

    MSG Msg;
    BOOL running = TRUE;
    int FPS = 120;
    double deltaTime = 0;
    double start;

    while (running) {
        start = time();

        // Process all pending messages first
        while (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
            if (Msg.message == WM_QUIT) {
                running = FALSE;
                break;
            }
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }

        HDC hdc = GetDC(hwnd);
        window_setHDC(&hdc);

        simulator_update();
        window_update();

        ReleaseDC(hwnd, hdc);

        deltaTime = time() - start;
        Sleep(max(0, (1.0 / FPS - deltaTime)) * 1000);
        deltaTime = time() - start;
    }
    simulator_end();
    window_end();

    return Msg.wParam;
}
