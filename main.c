#include <locale.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"
#include "player.h"
#include "battle.h"
#include "dungeon.h"

static void menu(void) {
    print("[0] Выход");
    print("[1] Создать / пересоздать персонажа");
    print("[2] Показать информацию о персонаже");
    print("[3] Тренировочный бой");
    print("[4] Освободить память");
    print("[5] Войти в подземелье");
    print("[6] Сохраниться на диск");
    print("[7] Загрузить сохранение");
    print("[8] Статы");
}

static void createOrRecreate(Player **pp) {
    if (playerExists(*pp)) {
        print("Пересоздаем персонажа!");
        freePlayer(pp);
    }
    char name[MAX_NAME_LEN + 1];
    readStrLtd("Введите имя персонажа: ", name, MAX_NAME_LEN);
    *pp = createPlayer(name);
    print("Персонаж '%s' создан", (*pp)->name);
}

int main(int argc, char* argv[]) {
    srand((unsigned) time(NULL));
    setlocale(LC_ALL, "Rus");

    Player *player = NULL;

    if (argc > 1) {
        Player *loaded = loadPlayer(argv[1]);
        if (loaded) player = loaded;
    }

    while (true) {
        menu();
        size_t choice = readMenuChoice();

        switch (choice) {
            case 0:
                if (playerExists(player)) {
                    print("Чистим память");
                    print("Выход");
                    freePlayer(&player);
                } else {
                    print("Чистить ничего");
                    print("Выход");
                }
                return 0;
            case 1:
                createOrRecreate(&player);
                break;
            case 2:
                if (!playerExists(player)) print("Персонаж не создан");
                else printPlayerInfo(player);
                break;
            case 3:
                if (!playerExists(player)) print("Персонаж не создан");
                else training(&player);
                break;
            case 4:
                if (!playerExists(player)) print("Персонаж не создан");
                else freePlayer(&player);
                break;
            case 5:
                if (!playerExists(player)) print("Персонаж не создан");
                else enter_dungeon(&player);
                break;
            case 6:
                if (!playerExists(player)) print("Персонаж не создан");
                else savePlayer(player);
                break;
            case 7:
                if (playerExists(player)) {
                    print("Текущий персонаж будет удален перед загрузкой. Продолжить? [1] Да [2] Нет");
                    if (readMenuChoice() != 1) break;
                    freePlayer(&player);
                }
                Player *loaded = loadPlayer(argv[1]);
                if (loaded) player = loaded;
                break;
            case 8:
                if (!playerExists(player)) print("Персонаж не создан");
                else printStatistics(player);
                break;
            default:
                illst();
                break;
        }
    }
}