#ifndef MAP_H
#define MAP_H

#include <stddef.h>

typedef struct Map {
    int width;
    int height;
    char *data; /* width * height */
} Map;

/* Создать и заполнить карту (границы '#', внутри '.') */
Map* create_map(int width, int height);

/* Проверка, можно ли пройти на клетку */
int map_is_walkable(const Map *m, int x, int y);

/* Освободить карту и напечатать адрес */
void free_map(Map **m);

/* получить и установить клетку */
char map_get(const Map *m, int x, int y);
void map_set(Map *m, int x, int y, char v);

/* отрисовать карту с позицией героя (знак '@') */
void draw_map(const Map *m, int heroX, int heroY);

/* заполнение сущностями согласно вероятностям */
void populate_map(Map *m, int depth);

#endif
