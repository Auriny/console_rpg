#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdbool.h>

void print(const char *format, ...);

void illst(void);

size_t readMenuChoice();

void readStrLtd(const char *prompt, char *buffer, size_t maxLen);

int readIntRange(const char *prompt, int min, int max);

int random(int min, int max);

void sleep(unsigned int ms);

int readKey(void);

#endif
