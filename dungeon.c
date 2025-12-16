#include "dungeon.h"
#include "utils.h"
#include "battle.h"
#include <time.h>

static void place_hero_random(Map *m, int *hx, int *hy) {
    for (int tries = 0; tries < 1000; ++tries) {
        int x = random(1, m->width - 2);
        int y = random(1, m->height - 2);
        if (map_get(m, x, y) == 'X') {
            *hx = x;
            *hy = y;
            return;
        }
    }
    //фолбек типа
    *hx = 1;
    *hy = 1;
}

static bool handle_cell(Player **ppPlayer, Map *m, int x, int y, int *depth, int *heroX, int *heroY, bool isLava) {
    if (! ppPlayer || !*ppPlayer) return false;
    Player *player = *ppPlayer;

    player->stats.steps++;

    char c = map_get(m, x, y);
    switch (c) {
        case '#': // стена
            return false;
        case 'X': //ход
            *heroX = x; *heroY = y;
            return true;
        case '+': //аптечка
            player->hp += 20;
            if (player->hp > player->maxHp) player->hp = player->maxHp;
            print("Подобрана аптечка: +20 HP (теперь %d/%d)", player->hp, player->maxHp);

            player->stats.medkitsCollected++;

            map_set(m, x, y, 'X');
            *heroX = x; *heroY = y;
            return true;
        case '~': { // костер
            player->hp += CAMP_HEAL;
            if (player->hp > player->maxHp) player->hp = player->maxHp;
            player->bonusAttack += CAMP_BONUS;
            print("Костёр: восстановлено %d HP, баф +%d к атаке на следующий бой.", CAMP_HEAL, CAMP_BONUS);

            player->stats.entitiesCollected++;

            map_set(m, x, y, 'X');
            *heroX = x; *heroY = y;
            return true;
        }
        case 'D': { //спуск на уровень ниже
            print("Спускаемся вниз...");
            (*depth)++;
            if (*depth > player->stats.maxFloor) player->stats.maxFloor = *depth;
            return false;
        }
        case 'U': { // лестница вверх
            print("Найдена лестница вверх! Выходим из подземелья.");
            print("Итог похода: уровень %d, опыт %d, голда %d", player->level, player->xp, player->gold);
            return false;
        }
        case 'M': { //монстрик
            print("Монстр нападает!");
            if (isLava) print("! АКТИВНА ЛАВА: НАГРАДЫ УДВОЕНЫ !");

            Player *enemy = createEnemy(*depth);
            if (!enemy) {
                print("Не удалось создать врага, продолжаем.");
                return false;
            }
            print("Бой: %s (Вы) vs %s (Монстр)", player->name, enemy->name);

            BattleResult res = battle(player, enemy);

            if (res == WIN) {
                int xp = enemy->xp;
                int gold = enemy->gold;

                if (isLava) {
                    xp *= 2;
                    gold *= 2;
                }

                player->xp += xp;
                player->gold += gold;
                print("Вы победили: +%d XP, +%d золота.", xp, gold);
                map_set(m, x, y, 'X');
                *heroX = x; *heroY = y;
                freeEnemy(&enemy);
                return true;
            } else {
                print("Вы пали в бою. . . ");
                if (player->xp >= 10) {
                    print("Хотите воскреснуть? (требует немного опыта и голды)");
                    print("[1] Да  [2] Нет");
                    size_t ch = readMenuChoice();
                    if (ch == 1) {
                        int xpCost = player->xp / 10;
                        if (xpCost < 10) xpCost = 10;
                        int goldCost = (player->gold >= 10) ? (player->gold / 2) : player->gold;
                        player->xp -= xpCost;
                        player->gold -= goldCost;
                        player->hp = player->maxHp;
                        print("Воскресье! %d XP и %d голды потрачено.", xpCost, goldCost);
                        freeEnemy(&enemy);
                        return true;
                    }
                }
                freeEnemy(&enemy);
                print("Смэрть в нищите. Удаляем персонажа.");
                freePlayer(ppPlayer);
                return false;
            }
        }
        default:
            *heroX = x; *heroY = y;
            return true;
    }
}

void enter_dungeon(Player **ppPlayer) {
    if (!ppPlayer || !*ppPlayer) {
        print("Создайте персонажа, прежде чем направиться в подземелье!");
        return;
    }

    Player *player = *ppPlayer;
    int depth = 1;
    if (depth > player->stats.maxFloor) player->stats.maxFloor = depth;

    Map *map = create_map(DUNGEON_MAP_W, DUNGEON_MAP_H);
    if (!map) {
        print("анлак");
        return;
    }
    populate_map(map, depth);

    int heroX = 1, heroY = 1;
    place_hero_random(map, &heroX, &heroY);

    bool isLava = (random(1, 100) <= LAVA_CHANCE_PERCENT);
    time_t floorStartTime = time(NULL);

    if (isLava) {
        print("!!! ПОЛ ЭТО ЛАВА !!!");
        print("У вас есть %d секунд, чтобы покинуть этаж (найти U или D).", LAVA_TIME_LIMIT_SEC);
        print("Награды за монстров удвоены, но если время выйдет - вы умрете.");
    }

    bool running = true;
    while (running && playerExists(player)) {
        time_t currentTime = time(NULL);
        if (isLava) {
            double elapsed = difftime(currentTime, floorStartTime);
            if (elapsed > LAVA_TIME_LIMIT_SEC) {
                print("Вы сгорели");
                free_map(&map);
                freePlayer(ppPlayer); // Смерть
                return;
            }
        }

        print("-===- Подземелье: уровень %d -===-", depth);
        draw_map(map, heroX, heroY);
        print("[W] Вверх  [S] Вниз  [A] Влево  [D] Вправо  [P] Статы  [Q] Выход из подземелья");

        int key = 0;
        while (key == 0 || key == '\n' || key == '\r') {
            key = readKey();
        }

        if (key == KEY_P) {
            printPlayerInfo(player);
            continue;
        }

        if (key == KEY_Q) {
            print("Выход из подземелья по желанию.");
            break;
        }

        int nx = heroX, ny = heroY;
        if (key == KEY_W) ny--;
        else if (key == KEY_S) ny++;
        else if (key == KEY_A) nx--;
        else if (key == KEY_D) nx++;

        if (nx < 0 || ny < 0 || nx >= map->width || ny >= map->height) {
            print("Туда нельзя.");
            continue;
        }

        bool movedOrHandled = handle_cell(ppPlayer, map, nx, ny, &depth, &heroX, &heroY, isLava);

        if (!playerExists(*ppPlayer)) {
            print("Персонаж удалён, выходим из подземелья.");
            running = false;
            break;
        }

        if (!movedOrHandled) {
            char cell = map_get(map, nx, ny);
            if (cell == 'D') {

                time_t endTime = time(NULL);
                double floorTime = difftime(endTime, floorStartTime);
                player->stats.totalPlayTime += floorTime;
                if (floorTime < player->stats.bestFloorTime) {
                    player->stats.bestFloorTime = floorTime;
                }

                free_map(&map);
                map = create_map(DUNGEON_MAP_W, DUNGEON_MAP_H);
                if (! map) {
                    print("Не удалось создать следующий уровень.");
                    running = false;
                    break;
                }
                populate_map(map, depth);
                place_hero_random(map, &heroX, &heroY);
                print("Новый уровень: %d", depth);

                isLava = (random(1, 100) <= LAVA_CHANCE_PERCENT);
                floorStartTime = time(NULL);
                if (isLava) {
                    print("!!! ПОЛ ЭТО ЛАВА !!!");
                    print("Бегите! %d секунд!", LAVA_TIME_LIMIT_SEC);
                }

                continue;
            } else if (cell == 'U') {
                time_t endTime = time(NULL);
                double floorTime = difftime(endTime, floorStartTime);
                player->stats.totalPlayTime += floorTime;
                if (floorTime < player->stats.bestFloorTime) {
                    player->stats.bestFloorTime = floorTime;
                }

                print("Выход наверх найден. Конец похода.");
                running = false;
                break;
            } else {
                continue;
            }
        }
    }

    free_map(&map);
    if (playerExists(player)) {
        print("Вы вышли из подземелья! Итог: уровень %d, опыт %d, голдп %d", player->level, player->xp, player->gold);
    }
}