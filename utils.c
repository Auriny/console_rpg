#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void print(const char *format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    wchar_t wbuffer[1024];
    MultiByteToWideChar(CP_UTF8, 0, buffer, -1, wbuffer, 1024);

    DWORD written;
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), wbuffer, wcslen(wbuffer), &written, NULL);
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), L"\n", 1, &written, NULL);
}

void illst(void) {
    print("Неизвестное действие!");
}