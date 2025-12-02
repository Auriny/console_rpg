#include <locale.h>
#include <stdio.h>
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
    // я бы кнш мог сделать на букву e вход в данж,
    // но мне бы тогда пришлось переделать всю структуру мейна
    // а я очень не хочу переделывать всю структуру мейна
}

static void createOrRecreate(Player **pp) {
    if (playerExists(*pp)) {
        print("Пересоздаем персонажа!");
        freePlayer(pp);
    }
    char name[MAX_NAME_LEN + 1];
    readStrLtd("Введите имя персонажа (до 32 символов): ", name, MAX_NAME_LEN);
    *pp = createPlayer(name);
    print("Персонаж '%s' создан", (*pp)->name);
}

int main(void) {
    srand((unsigned)time(NULL));
    setlocale(LC_ALL, "Rus");

    Player *player = NULL;

    while (true) {
        menu();
        size_t choice = readMenuChoice();

        switch (choice) {
            // щас она принимает только инт значения (size_t по факту инт ток побольше)
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
                else enter_dungeon(&player); /* NEW */
                break;
            default:
                illst();
                break;
        }
    }
}
