//
// Created by eitan on 9/25/2025.
//

#include "../include/PowderSimulator.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "../include/Window.h"
#include "../include/Util.h"

#define TYPES_COUNT 6

typedef struct CellValue {
    uint8_t type;
    uint8_t checked;
    uint8_t moved;
} CellValue_t;

static uint8_t** cells;
static int width, height;
static COLORREF type_to_color[TYPES_COUNT] = { RGB(25, 25, 25), RGB(125, 125, 62.5), RGB(50, 75, 150), RGB(60, 60, 60), RGB(10, 10, 10), RGB(182, 175, 225) };
static int selected_type = 1;
static bool rightClicked = false;
static int radius = 1;

uint8_t** create_cells(int w, int h) {
    uint8_t** c = malloc(h * sizeof(uint8_t*));

    for (int i = 0; i < h; i++) {
        c[i] = (uint8_t*)malloc(w * sizeof(uint8_t));
        for (int j = 0; j < w; j++) {
            c[i][j] = 0;
        }
    }

    return c;
}

void free_cells(uint8_t** c, int h) {
    for (int i = 0; i < h; i++) {
        free(c[i]);
    }
    free(c);
}

int getCell(uint8_t** c, int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height)
        return c[y][x];
    return -1;
}

void setCell(uint8_t** c, int x, int y, uint8_t value) {
    if (x >= 0 && x < width && y >= 0 && y < height)
        c[y][x] = value;
}

CellValue_t getCellValues(uint8_t** c, int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height)
        return (CellValue_t){ c[y][x] & 0b00111111, c[y][x] >> 7, (c[y][x] & 0b01000000) >> 6 };
    return (CellValue_t){ -1, -1, -1 };
}

void setCellValues(uint8_t** c, int x, int y, uint8_t type, uint8_t checked, uint8_t moved) {
    if (x >= 0 && x < width && y >= 0 && y < height)
        c[y][x] = ((checked & 1) << 7) | ((moved & 1) << 6) | (type & 0b00111111);
}

void shuffle(POINT* arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        POINT tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

void draw() {
    window_drawBackground(type_to_color[0]);

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
            setCell(cells, x, y, 0);
        }
    }

    window_preFillBrushes(type_to_color, TYPES_COUNT);

    srand(time());
}

bool step_cell(int x, int y, uint8_t** cells, uint8_t** new_cells) {
    POINT positions[10];
    int pos_count = 0;
    CellValue_t cell_value = getCellValues(cells, x, y);

    if (cell_value.type == 0) {
        return false;
    }

    if (cell_value.checked == 1) {
        return cell_value.moved;
    }

    switch (cell_value.type) {
        case 0: // Blank
            break;

        case 1: // Sand
            positions[0] = (POINT){ x, y + 1 };
            positions[1] = (POINT){ x - 1, y + 1 };
            positions[2] = (POINT){ x + 1, y + 1 };
            pos_count = 3;
            break;

        case 2: // Water
            positions[0] = (POINT){ x, y + 1 };
            positions[1] = (POINT){ x - 1, y + 1 };
            positions[2] = (POINT){ x + 1, y + 1 };
            positions[3] = (POINT){ x - 1, y };
            positions[4] = (POINT){ x + 1, y };
            pos_count = 5;
            break;

        case 3: // Stone
            positions[0] = (POINT){ x, y + 1 };
            pos_count = 1;
            break;

        case 4: // Metal
            break;

        case 5: // Gas
            positions[0] = (POINT){ x, y - 1 };
            positions[1] = (POINT){ x - 1, y - 1 };
            positions[2] = (POINT){ x + 1, y - 1 };
            positions[3] = (POINT){ x - 1, y };
            positions[4] = (POINT){ x + 1, y };
            pos_count = 5;

            shuffle(positions, 3);
            shuffle(&positions[3], 2);
            break;

        default:
            break;
    }

    bool moved = false;
    for (int i = 0; i < pos_count; i++) {
        if (getCell(cells, positions[i].x, positions[i].y) == -1)
            continue;

        CellValue_t target_cell_value = getCellValues(cells, positions[i].x, positions[i].y);
        uint8_t target_cell_new_value = getCell(new_cells, positions[i].x, positions[i].y);

        if (target_cell_value.type == 0) {
            if (target_cell_new_value == 0) {
                setCell(new_cells, positions[i].x, positions[i].y, cell_value.type);
                moved = true;
                break;
            }
        }
        else {
            bool didMove = target_cell_value.moved;
            if (!target_cell_value.checked) {
                setCellValues(cells, x, y, cell_value.type, 1, cell_value.moved);
                didMove = step_cell(positions[i].x, positions[i].y, cells, new_cells);
            }

            if (didMove && target_cell_new_value == 0) {
                setCell(new_cells, positions[i].x, positions[i].y, cell_value.type);
                moved = true;
                break;
            }
        }
    }
    if (!moved) {
        setCell(new_cells, x, y, cell_value.type);
    }
    setCellValues(cells, x, y, cell_value.type, 1, moved ? 1 : 0);

    return moved;
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
            selected_type = selected_type % (TYPES_COUNT - 1) + 1;
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

    uint8_t** new_cells = create_cells(width, height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            step_cell(x, y, cells, new_cells);
        }
    }

    free_cells(cells, height);
    cells = new_cells;

    draw();
}

void simulator_end() {
    free_cells(cells, height);
}