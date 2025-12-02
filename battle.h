#include "player.h"

#ifndef BATTLE_H
#define BATTLE_H

#define TURN_DELAY_MS 400

typedef enum BattleResult {
    WIN = 0,
    DEAD = 1
} BattleResult;

Player* create(void);
Player* createEnemy(int depth);

void freeEnemy(Player **pp);

void training(Player **ppPlayer);

BattleResult battle(Player *player, Player *enemy);

#endif
