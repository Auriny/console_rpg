#ifndef PLAYER_H
#define PLAYER_H

#include <stddef.h>
#include <stdbool.h>

#define MAX_NAME_LEN 32

typedef struct Player {
    char name[MAX_NAME_LEN + 1];
    int maxHp;
    int hp;
    int attack;
    int defense;
    int level;
    int xp;
    int gold;

    int dodge;

    int bonusAttack;
} Player;

Player* createPlayer(const char *name);
void freePlayer(Player **pp);

void printPlayerInfo(const Player *p);

static inline bool playerExists(const Player *p) {
    return p != NULL;
}

#endif
