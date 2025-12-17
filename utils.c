#include "utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

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

static void trimNewline(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') s[n - 1] = '\0';
}

static bool readLine(char *buf, size_t cap) {
    if (!fgets(buf, (int)cap, stdin)) {
        clearerr(stdin);
        return false;
    }
    trimNewline(buf);
    return true;
}

size_t readMenuChoice() {
    char line[256];
    while (true) {
        if (!readLine(line, sizeof(line))) {
            print("Ошибка ввода, попробуйте снова.");
            continue;
        }
        if (line[0] == '\0') {
            continue;
        }
        char *endptr = NULL;
        const unsigned long v = strtoul(line, &endptr, 10);
        if (!endptr || *endptr != '\0') {
            print("Ожидается целое число, повторите ввод.");
            continue;
        }
        return v;
    }
}

void readStrLtd(const char *prompt, char *buffer, size_t maxLen) {
    char line[512];
    for (;;) {
        if (prompt && *prompt) print("%s", prompt);
        if (!readLine(line, sizeof(line))) {
            print("Ошибка ввода, попробуйте снова");
            continue;
        }
        const size_t len = strlen(line);
        if (len == 0) {
            print("Строка не должна быть пустой, повторите ввод");
            continue;
        }
        if (len > maxLen) {
            print("Слишком длинно (макс %zu символов). Повторите ввод", maxLen);
            continue;
        }
        strcpy(buffer, line);
        return;
    }
}

int random(int min, int max) {
    if (max < min) {
        int t = min;
        min = max;
        max = t;
    }
    const int span = max - min + 1;
    return min + rand() % span;
}

void sleep(unsigned int ms) {
    Sleep(ms);
}

int readKey(void) {
    int ch = _getch(); //эта тема не очень сильно работает, я не разобрался :(
    // хотел сделать чтобы можно было получать символ с клавиатуры без нажатия на enter
    //типа реально тыкаешь w и оно идет вперед а не как пошаговое непонятно что
    //но увы)
    if (ch == 0 || ch == 0xE0) {
        int ch2 = _getch();
        (void)ch2;
        return 0;
    }
    if (ch >= 'a' && ch <= 'z') ch = ch - 'a' + 'A';
    return ch;
}
