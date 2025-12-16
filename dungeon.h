#ifndef DUNGEON_H
#define DUNGEON_H

#include "player.h"
#include "map.h"

#define KEY_W 'W'
#define KEY_A 'A'
#define KEY_S 'S'
#define KEY_D 'D'
#define KEY_P 'P'
#define KEY_Q 'Q'

#define DUNGEON_MAP_W 20
#define DUNGEON_MAP_H 20

#define CAMP_HEAL 20
#define CAMP_BONUS 5

#define LAVA_CHANCE_PERCENT 20
#define LAVA_TIME_LIMIT_SEC 30

void enter_dungeon(Player **ppPlayer);

#endif