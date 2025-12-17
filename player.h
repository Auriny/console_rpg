#ifndef PLAYER_H
#define PLAYER_H

#include <stddef.h>
#include <stdbool.h>

#define MAX_NAME_LEN 32

typedef struct Statistics {
    long long steps;
    int medkitsCollected;
    int entitiesCollected;
    int enemiesKilled;
    int maxFloor;
    int maxDmgDealt;
    int maxDmgTaken;
    unsigned long bestFloorTime;
    unsigned long totalPlayTime;
    int skillUsed;
} Statistics;

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

    Statistics stats;
} Player;

Player* createPlayer(const char *name);
void freePlayer(Player **pp);

void printPlayerInfo(const Player *p);
void printStatistics(const Player *p);

void lvlUp(Player *p);

bool savePlayer(const Player *p);
Player* loadPlayer(const char* fileName);

static bool playerExists(const Player *p) {
    return p != NULL;
}

#endif