//
// Created by eitan on 9/27/2025.
//

#include "../include/Util.h"
#include <windows.h>

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