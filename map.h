#ifndef MAP_H
#define MAP_H

#include <stddef.h>

typedef struct Map {
    int width;
    int height;
    char *data;
} Map;

Map* create_map(int width, int height);

void free_map(Map **m);

char map_get(const Map *m, int x, int y);
void map_set(Map *m, int x, int y, char v);

void draw_map(const Map *m, int heroX, int heroY);

void populate_map(Map *m, int depth);

#endif
