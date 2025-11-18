#ifndef BATTLE_H
#define BATTLE_H

#include "player.h"

#define TURN_DELAY_MS 400

typedef enum BattleResult {
    BATTLE_PLAYER_WIN = 0,
    BATTLE_PLAYER_DEAD = 1
} BattleResult;

Player* create(void);
void freeEnemy(Player **pp);

void training(Player **ppPlayer);

#endif