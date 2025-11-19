#ifndef BATTLE_H
#define BATTLE_H

#include "player.h"

#define TURN_DELAY_MS 400

typedef enum BattleResult {
    WIN = 0,
    DEAD = 1
} BattleResult;

Player* create(void);

void freeEnemy(Player **pp);

void training(Player **ppPlayer);

#endif