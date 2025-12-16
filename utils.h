#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdbool.h>

#define SAVE_FILENAME "save.txt"

void print(const char *format, ...);

void illst(void);

size_t readMenuChoice();

void readStrLtd(const char *prompt, char *buffer, size_t maxLen);

int random(int min, int max);

void sleep(unsigned int ms);

int readKey(void);

#endif
