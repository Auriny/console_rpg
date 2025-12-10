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

    p->bonusAttack = 0;

    print("Структура игрока создана по адресу %p", (void*)p);
    return p;
}

static int xp_required_for(int lvl) {
    return (int)(50 * lvl * lvl + 50 * lvl);
}

void lvlUp(Player *p) {
    int need = xp_required_for(p->level);

    while (p->xp >= need) {
        p->level++;

        p->attack += 2 + p->level / 2;
        p->defense += 1 + p->level / 3;
        p->maxHp += 5 + p->level;

        print("Новый уровень!");
        need = xp_required_for(p->level);
    }
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
    print("  Бонус: %d", p->bonusAttack);
}