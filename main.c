#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdarg.h>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#include "Player.c"
#include "battle.c"
#include "utils.c"

void menu() {
    print("[0] Выход");
    print("[1] Создать / пересоздать персонажа");
    print("[2] Показать информацию о персонаже");
    print("[3] Тренировочный бой");
    print("[4] Очистка памяти");
}

void createCharacter() {
    readName();
}

int main(void) {
    srand(time(NULL));
    setlocale(LC_ALL, "Rus");

    size_t choice;
    while (1) {
        menu();
        scanf("%zu", &choice);

        switch(choice) {
            case 0: return 0;
            // case 1: helloWorld(); break;
            // case 2: vars(); break;
            // case 3: convertDegrees(); break;
            // case 4: fibonacci(); break;

            default: illst(); break;
        }
    }

    return 0;
}