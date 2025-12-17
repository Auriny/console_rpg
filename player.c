#include "player.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
    } else strcpy(p->name, "Игрок");

    p->maxHp = 100;
    p->hp = p->maxHp;
    p->attack = 15;
    p->defense = 3;
    p->level = 1;
    p->xp = 0;
    p->gold = 0;

    p->dodge = 35;

    p->bonusAttack = 0;

    memset(&p->stats, 0, sizeof(p->stats));
    p->stats.bestFloorTime = 9999999; //костыль? не знаю

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

        print("Новый уровень! :3");
        need = xp_required_for(p->level);
    }
}

void freePlayer(Player **pp) {
    if (!pp || !*pp) return;
    Player *p = *pp;
    printStatistics(p);
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
    print("  Бонус к некст атаке: %d", p->bonusAttack);
}

void printStatistics(const Player *p) {
    if (!p) return;
    const Statistics *s = &p->stats;
    print("=== СТАТИСТИКА ===");
    print("Шагов пройдено: %lld", s->steps);
    print("Аптечек собрано: %d", s->medkitsCollected);
    print("Костров собрано: %d", s->entitiesCollected);
    print("Врагов убито: %d", s->enemiesKilled);
    print("Макс. этаж: %d", s->maxFloor);
    print("Макс. урон (нанесен): %d", s->maxDmgDealt);
    print("Макс. урон (получен): %d", s->maxDmgTaken);
    if (s->bestFloorTime > 900000.0) {
        print("Рекорд этажа: нет данных");
    } else print("Рекорд этажа: %d сек", s->bestFloorTime);
    print("Общее время игры: %d сек", s->totalPlayTime);
    print("Успешных уклонений: %d", s->skillUsed);
    print("================");
}

bool savePlayer(const Player *p) {
    FILE *f = fopen(SAVE_FILENAME, "w");
    if (!f) {
        print("Ошибка открытия файла для записи!");
        return false;
    }

    fprintf(f, "%s\n", p->name);
    fprintf(f, "%d %d %d %d %d %d %d %d\n",
            p->maxHp, p->hp, p->attack, p->defense,
            p->level, p->xp, p->gold, p->dodge);

    const Statistics *s = &p->stats;
    fprintf(f, "%lld %d %d %d %d %d %d %.2lu %.2lu %d\n",
            s->steps, s->medkitsCollected, s->entitiesCollected,
            s->enemiesKilled, s->maxFloor, s->maxDmgDealt,
            s->maxDmgTaken, s->bestFloorTime, s->totalPlayTime,
            s->skillUsed);

    fclose(f);
    print("Персонаж успешно сохранен в %s", SAVE_FILENAME);
    return true;
}

Player* loadPlayer(const char* fileName) {
    FILE *f = fileName == NULL ? fopen(SAVE_FILENAME, "r") : fopen(fileName, "r");
    if (!f) {
        print("Файл сохранения не найден!");
        return NULL;
    }

    Player *p = malloc(sizeof(Player));
    if (!p) {
        fclose(f);
        return NULL;
    }
    memset(p, 0, sizeof(*p));

    char buffer[256];
    if (fgets(buffer, sizeof(buffer), f)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
        strncpy(p->name, buffer, MAX_NAME_LEN);
    } else {
        free(p);
        fclose(f);
        return NULL;
    }

    if (fscanf(f, "%d %d %d %d %d %d %d %d",
               &p->maxHp, &p->hp, &p->attack, &p->defense,
               &p->level, &p->xp, &p->gold, &p->dodge) != 8) {
        print("Ошибка чтения характеристик персонажа!");
        free(p); fclose(f); return NULL;
    }

    p->bonusAttack = 0;

    if (fscanf(f, "%lld %d %d %d %d %d %d %lu %lu %d",
               &p->stats.steps, &p->stats.medkitsCollected, &p->stats.entitiesCollected,
               &p->stats.enemiesKilled, &p->stats.maxFloor, &p->stats.maxDmgDealt,
               &p->stats.maxDmgTaken, &p->stats.bestFloorTime, &p->stats.totalPlayTime,
               &p->stats.skillUsed) != 10) {
        print("Статистика не прочитана или повреждена, сброс");
        memset(&p->stats, 0, sizeof(p->stats));
        p->stats.bestFloorTime = 999999;
    }

    fclose(f);
    print("Персонаж %s загружен! Адрес: %p", p->name, (void*)p);
    return p;
}