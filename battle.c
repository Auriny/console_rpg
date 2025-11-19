#include "battle.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static void name(char *out, size_t cap) {
    int len = random(3, 10);
    if ((int)cap <= len) len = (int)cap - 1;
    for (int i = 0; i < len; ++i) {
        int r = random(0, 25);
        out[i] = (char)('a' + r);
    }
    if (len > 0) out[0] = (char)toupper((unsigned char)out[0]);
    out[len] = '\0';
}

Player* create(void) {
    Player *e = malloc(sizeof(Player));
    if (!e) {
        print("Не удалось выделить место под противника, чота с памятью");
        return NULL;
    }
    memset(e, 0, sizeof(*e));

    name(e->name, sizeof(e->name));
    e->level = random(1, 5);
    e->maxHp = random(3, 15);
    e->hp = e->maxHp;
    e->attack = 10 * e->level;
    e->defense = 5 * e->level;

    e->xp = e->maxHp + e->attack + e->defense;
    e->gold = random(0, e->xp);

    e->dodge = random(0, 25);

    print("Противник %s создан по адресу %p (уровень %d, HP %d, атака %d, защита %d)",
    e->name, e, e->level, e->hp, e->attack, e->defense);
    return e;
}

void freeEnemy(Player **pp) {
    if (!pp || !*pp) return;
    Player *e = *pp;
    print("Структура противника по адресу %p выгружена", e);
    free(e);
    *pp = NULL;
}

static int baseDamage(const Player *att, const Player *def) {
    int dmg = att->attack - def->defense / 2;
    return dmg < 1 ? 1 : dmg;
}

static int roll(int percent) {
    if (percent <= 0) return 0;
    if (percent >= 100) return 1;
    int roll = random(1, 100);
    return roll <= percent;
}

static void logatt(const Player *att, const Player *def, int dmg, bool dodged) {
    if (dodged) {
        print("  >%s уклоняется от атаки %s!", def->name, att->name);
        return;
    }
    print("  >%s бьёт %s на %d урона.", att->name, def->name, dmg);
}

static BattleResult battle(Player *player, Player *enemy) {
    int step = 1;
    while (player->hp > 0 && enemy->hp > 0) {
        print("Ход %d:", step);

        bool dodged = roll(enemy->dodge);
        int dmg = 0;
        if (!dodged) {
            dmg = baseDamage(player, enemy);
            if (dmg < 1) dmg = 1; //без этого куска почему-то крашит
            enemy->hp -= dmg;
            if (enemy->hp < 0) enemy->hp = 0;
        }
        logatt(player, enemy, dmg, dodged);
        print("%s: %d/%d HP | %s: %d/%d HP", player->name, player->hp, player->maxHp, enemy->name, enemy->hp, enemy->maxHp);
        sleep(TURN_DELAY_MS);
        if (enemy->hp <= 0) break;

        dodged = roll(player->dodge);
        dmg = 0;
        if (!dodged) {
            dmg = baseDamage(enemy, player);
            if (dmg < 1) dmg = 1;
            player->hp -= dmg;
            if (player->hp < 0) player->hp = 0;
        }
        logatt(enemy, player, dmg, dodged);
        print("%s: %d/%d HP | %s: %d/%d HP", player->name, player->hp, player->maxHp, enemy->name, enemy->hp, enemy->maxHp);
        sleep(TURN_DELAY_MS);

        step++;
    }
    return player->hp > 0 ? WIN : DEAD;
}

static void win(Player *player, const Player *enemy) {
    print("Противник %s умэр. Победа!", enemy->name);
    int xp = enemy->xp;
    int gold = enemy->gold;
    player->xp += xp;
    player->gold += gold;
    print("Награда: +%d опыта, +%d голды.", xp, gold);
    print("Ваши итоги: опыт=%d, золото=%d", player->xp, player->gold);
}

static bool canResurrect(const Player *p) {
    return p->xp >= 10;
}

static bool tryResurrect(Player *p) {
    if (!canResurrect(p)) {
        print("Воскрешение невозможн! У вас недостаточно опыта (надо минимум 10)");
        return false;
    }
    int xpCost = p->xp / 10; // 10%
    if (xpCost < 10) xpCost = 10;
    int goldCost = (p->gold >= 10) ? (p->gold / 2) : p->gold;

    print("Вы умерли!");
    print("Воскреснуть? Стоимость: %d опыта и %d голды.", xpCost, goldCost);
    print("[1] Да  [2] Нет");
    size_t ch = readMenuChoice();
    if (ch == 1) {
        p->xp   -= xpCost;
        p->gold -= goldCost;
        p->hp = p->maxHp;
        print("Воскресье! Текущий опыт: %d, голды: %d", p->xp, p->gold);
        return true;
    }
    return false;
}

void training(Player **ppPlayer) {
    if (!ppPlayer || !*ppPlayer) {
        print("Сначала надо создать персонажа");
        return;
    }
    Player *player = *ppPlayer;
    Player *enemy = create();
    if (!enemy) {
        print("Не удалось начать бой! Противник не был создан");
        return;
    }

    print("Начинается тренировочный бой: %s (Вы) vs %s (Противник)", player->name, enemy->name);

    BattleResult res = battle(player, enemy);
    if (res == WIN) {
        win(player, enemy);
        freeEnemy(&enemy);
        return;
    }

    print("Смэрть");
    if (tryResurrect(player)) {
        freeEnemy(&enemy);
        return;
    }

    print("Удаляем...");
    freeEnemy(&enemy);
    freePlayer(ppPlayer);
}
