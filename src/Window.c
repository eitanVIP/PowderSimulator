//
// Created by eitan on 9/25/2025.
//

#include "../include/Window.h"

#include <stdio.h>

static HWND window_hwnd;
static HDC* window_hdc;
static HDC mem_hdc;
static HBITMAP hBitmap;
static SIZE window_size;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_SIZE:
            window_size = (SIZE){ LOWORD(lParam), HIWORD(lParam) };

            // Recreate backbuffer
            if (mem_hdc) {
                DeleteObject(hBitmap);
                DeleteDC(mem_hdc);
            }
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI window_setupWindow(HINSTANCE hInstance, int nCmdShow, HWND* hwndPtr) {
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "Powder Simulator";

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    window_hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        "Powder Simulator",
        "Powder Simulator",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1080, 1080,
        NULL, NULL, hInstance, NULL);

    if (window_hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    *hwndPtr = window_hwnd;

    ShowWindow(window_hwnd, nCmdShow);
    UpdateWindow(window_hwnd);

    return 0;
}

void window_setHDC(HDC* hdc) {
    window_hdc = hdc;

    DeleteObject(hBitmap);
    DeleteDC(mem_hdc);
    mem_hdc = CreateCompatibleDC(*hdc);
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = window_size.cx;
    bmi.bmiHeader.biHeight = -window_size.cy; // negative so origin is top-left
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* pixels;
    hBitmap = CreateDIBSection(mem_hdc, &bmi, DIB_RGB_COLORS, &pixels, NULL, 0);
    SelectObject(mem_hdc, hBitmap);
}

void window_drawBackground(COLORREF color) {
    HBRUSH blackBrush = CreateSolidBrush(color);
    window_drawRect(0, 0, window_size.cx, window_size.cy, color);
}

void window_drawRect(int x1, int y1, int x2, int y2, COLORREF color) {
    HBRUSH brush = CreateSolidBrush(color);
    HPEN nullPen = (HPEN)GetStockObject(NULL_PEN);

    HGDIOBJ oldBrush = SelectObject(mem_hdc, brush);
    HGDIOBJ oldPen   = SelectObject(mem_hdc, nullPen);

    Rectangle(mem_hdc, x1, y1, x2, y2);

    // Restore old objects
    SelectObject(mem_hdc, oldBrush);
    SelectObject(mem_hdc, oldPen);

    DeleteObject(brush);
}

static HBRUSH* brushes;
static HPEN nullPen;
void window_preFillBrushes(COLORREF colors[], int count) {
    brushes = malloc(count * sizeof(HBRUSH));

    for (int i = 0; i < count; i++) {
        brushes[i] = CreateSolidBrush(colors[i]);
    }

    nullPen = (HPEN)GetStockObject(NULL_PEN);
}

void window_drawRectPreFill(int x1, int y1, int x2, int y2, int brushNum) {
    HBRUSH brush = brushes[brushNum];
    SelectObject(mem_hdc, brush);
    SelectObject(mem_hdc, nullPen);
    Rectangle(mem_hdc, x1, y1, x2, y2);
}

void window_setPixel(int x, int y, COLORREF color) {
    SetPixelV(mem_hdc, x, y, color);
}

void window_drawLine(int x1, int y1, int x2, int y2, int width, COLORREF color) {
    HPEN pen = CreatePen(PS_SOLID, width, color);
    SelectObject(mem_hdc, pen);
    MoveToEx(mem_hdc, x1, y1, NULL);
    LineTo(mem_hdc, x2, y2);
    DeleteObject(pen);
}

void window_drawCircle(int x, int y, int radius, COLORREF color) {
    HBRUSH brush = CreateSolidBrush(color);
    SelectObject(mem_hdc, brush);
    Ellipse(mem_hdc, x - radius, y - radius, x + radius, y + radius);
    DeleteObject(brush);
}

SIZE window_getSize() {
    return window_size;
}

POINT window_getCursorPosition() {
    POINT p;
    if (GetCursorPos(&p)) {
        ScreenToClient(window_hwnd, &p);
    } else {
        printf("GetCursorPos failed!\n");
    }

    return p;
}

void window_update() {
    BitBlt(*window_hdc, 0, 0, window_size.cx, window_size.cy, mem_hdc, 0, 0, SRCCOPY);
}

void window_end() {
    DeleteObject(hBitmap);
    DeleteDC(mem_hdc);

    for (int i = 0; i < sizeof(brushes) / sizeof(brushes[0]); i++) {
        DeleteObject(brushes[i]);
    }

    DeleteObject(nullPen);
}