//
// Created by eitan on 9/25/2025.
//

#ifndef POWDERSIMULATOR_WINDOW_H
#define POWDERSIMULATOR_WINDOW_H
#include <windows.h>

int WINAPI window_setupWindow(HINSTANCE hInstance, int nCmdShow, HWND* hwnd);
void window_setHDC(HDC* hdc);
void window_drawBackground(COLORREF color);
void window_drawRect(int x1, int y1, int x2, int y2, COLORREF color);
void window_preFillBrushes(COLORREF colors[], int count);
void window_drawRectPreFill(int x1, int y1, int x2, int y2, int brushNum);
void window_setPixel(int x, int y, COLORREF color);
void window_drawLine(int x1, int y1, int x2, int y2, int width, COLORREF color);
void window_drawCircle(int x, int y, int radius, COLORREF color);
SIZE window_getSize();
POINT window_getCursorPosition();
void window_update();
void window_end();

#endif //POWDERSIMULATOR_WINDOW_H