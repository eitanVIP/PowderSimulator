//
// Created by eitan on 9/25/2025.
//

#include "../include/PowderSimulator.h"
#include <stdio.h>
#include "../include/Window.h"
#include <stdbool.h>

static unsigned char** cells;
static int width, height;
static COLORREF type_to_color[5] = { RGB(25, 25, 25), RGB(125, 125, 62.5), RGB(50, 75, 150), RGB(60, 60, 60), RGB(10, 10, 10) };
static int selected_type = 1;
static bool rightClicked = false;
static int radius = 1;

unsigned char** create_cells(int w, int h) {
    unsigned char** c = malloc(h * sizeof(unsigned char*));

    for (int i = 0; i < h; i++) {
        c[i] = (unsigned char*)malloc(w * sizeof(int));
        for (int j = 0; j < w; j++) {
            c[i][j] = 0;
        }
    }

    return c;
}

void free_cells(unsigned char** c, int h) {
    for (int i = 0; i < h; i++) {
        free(c[i]);
    }
    free(c);
}

int getCell(unsigned char** c, int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height)
        return c[y][x];
    return -1;
}

void setCell(unsigned char** c, int x, int y, int value) {
    if (x >= 0 && x < width && y >= 0 && y < height)
        c[y][x] = value;
}

void draw() {
    window_drawBackground(RGB(25, 25, 25));

    double cell_width = window_getSize().cx / (double)width;
    double cell_height = window_getSize().cy / (double)height;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            window_drawRectPreFill((int)(x * cell_width), (int)(y * cell_height), (int)((x + 1) * cell_width) + 1, (int)((y + 1) * cell_height) + 1, getCell(cells, x, y));
        }
    }
}

void simulator_start(int width_p, int height_p) {
    width = width_p;
    height = height_p;

    cells = create_cells(width, height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // setCell(cells, x, y, rand() % 5);
            setCell(cells, x, y, 0);
        }
    }

    window_preFillBrushes(type_to_color, 5);
}

void simulator_update() {
    if (GetAsyncKeyState('1') & 0x8000) {
        radius = 1;
    } else if (GetAsyncKeyState('2') & 0x8000) {
        radius = 2;
    } else if (GetAsyncKeyState('3') & 0x8000) {
        radius = 3;
    } else if (GetAsyncKeyState('4') & 0x8000) {
        radius = 4;
    } else if (GetAsyncKeyState('5') & 0x8000) {
        radius = 5;
    } else if (GetAsyncKeyState('6') & 0x8000) {
        radius = 6;
    } else if (GetAsyncKeyState('7') & 0x8000) {
        radius = 7;
    } else if (GetAsyncKeyState('8') & 0x8000) {
        radius = 8;
    } else if (GetAsyncKeyState('9') & 0x8000) {
        radius = 9;
    }

    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
        if (!rightClicked) {
            selected_type = selected_type % 4 + 1;
            rightClicked = true;
        }
    } else {
        rightClicked = false;
    }

    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        POINT pos = window_getCursorPosition();
        double cell_width = window_getSize().cx / (double)width;
        double cell_height = window_getSize().cy / (double)height;

        pos.x = pos.x / cell_width;
        pos.y = pos.y / cell_height;

        int r = radius;
        int cx = pos.x;
        int cy = pos.y;
        int r2 = r * r;
        for (int y = cy - r; y <= cy + r; y++) {
            for (int x = cx - r; x <= cx + r; x++) {
                int dx = x - cx;
                int dy = y - cy;
                if (dx * dx + dy * dy <= r2) {
                    setCell(cells, x, y, selected_type);
                }
            }
        }
    }

    unsigned char** new_cells = create_cells(width, height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            switch (getCell(cells, x, y)) {
                case 0:
                    break;

                case 1:
                    if (getCell(cells, x, y + 1) == 0 && getCell(new_cells, x, y + 1) == 0) {
                        setCell(new_cells, x, y + 1, 1);
                    }
                    else if (getCell(cells, x - 1, y + 1) == 0 && getCell(new_cells, x - 1, y + 1) == 0) {
                        setCell(new_cells, x - 1, y + 1, 1);
                    }
                    else if (getCell(cells, x + 1, y + 1) == 0 && getCell(new_cells, x + 1, y + 1) == 0) {
                        setCell(new_cells, x + 1, y + 1, 1);
                    }
                    else {
                        setCell(new_cells, x, y, 1);
                    }
                    break;

                case 2:
                    if (getCell(cells, x, y + 1) == 0 && getCell(new_cells, x, y + 1) == 0) {
                        setCell(new_cells, x, y + 1, 2);
                    }
                    else if (getCell(cells, x - 1, y + 1) == 0 && getCell(new_cells, x - 1, y + 1) == 0) {
                        setCell(new_cells, x - 1, y + 1, 2);
                    }
                    else if (getCell(cells, x + 1, y + 1) == 0 && getCell(new_cells, x + 1, y + 1) == 0) {
                        setCell(new_cells, x + 1, y + 1, 2);
                    }
                    else if (getCell(cells, x - 1, y) == 0 && getCell(new_cells, x - 1, y) == 0) {
                        setCell(new_cells, x - 1, y, 2);
                    }
                    else if (getCell(cells, x + 1, y) == 0 && getCell(new_cells, x + 1, y) == 0) {
                        setCell(new_cells, x + 1, y, 2);
                    }
                    else {
                        setCell(new_cells, x, y, 2);
                    }
                    break;

                case 3:
                    if (getCell(cells, x, y + 1) == 0 && getCell(new_cells, x, y + 1) == 0) {
                        setCell(new_cells, x, y + 1, 3);
                    }
                    else {
                        setCell(new_cells, x, y, 3);
                    }
                    break;

                case 4:
                    setCell(new_cells, x, y, 4);
                    break;

                default:
                    break;
            }
        }
    }

    free_cells(cells, height);
    cells = new_cells;

    draw();
}

void simulator_end() {
    free_cells(cells, height);
}