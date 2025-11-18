#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "utils.c"

struct Player {
    char name[20];
    int maxHp;
    int hp;
    int damage;
    int protection;
    int level;
    int xp;
    int gold;
};

struct Player *playerPtr;

void allocatePlayerMemory() {
    playerPtr = (struct player*) malloc(sizeof(struct Player));
}

void freePlayerMemory() {
    free(playerPtr);
}

void readName() {
    char name[20];
    print("%s", "Введите никнейм игрока");
    scanf("%s", name);
    strcpy(playerPtr->name, name);
}