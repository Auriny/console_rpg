#include "player.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

Player* createPlayer(const char *name) {
    Player *p = malloc(sizeof(Player));
    if (!p) {
        print("Ошибка: не удалось выделить память под игрока");
        return NULL;
    }
    memset(p, 0, sizeof(*p));

    if (name && *name) {
        strncpy(p->name, name, MAX_NAME_LEN);
        p->name[MAX_NAME_LEN] = '\0';
    } else {
        strcpy(p->name, "Игрок");
    }

    p->maxHp = 100;
    p->hp = p->maxHp;
    p->attack = 15;
    p->defense = 3;
    p->level = 1;
    p->xp = 0;
    p->gold = 0;

    p->dodge = 5;

    print("Структура игрока создана по адресу %p", (void*)p);
    return p;
}

void freePlayer(Player **pp) {
    if (!pp || !*pp) return;
    Player *p = *pp;
    print("Структура игрока по адресу %p выгружена", (void*)p);
    free(p);
    *pp = NULL;
}

void printPlayerInfo(const Player *p) {
    if (!p) {
        print("Персонаж не создан");
        return;
    }
    print("Информация о персонаже:");
    print("  Имя: %s", p->name);
    print("  Лвл: %d", p->level);
    print("  Хп: %d / %d", p->hp, p->maxHp);
    print("  Атака: %d", p->attack);
    print("  Защита: %d", p->defense);
    print("  Опыт: %d", p->xp);
    print("  Голда: %d", p->gold);
    print("  Уклонение: %d%%", p->dodge);
}