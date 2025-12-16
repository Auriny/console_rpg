#include "map.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

#define MAP_MIN 3
#define MAP_MAX 20

static const int PROB_MONSTER = 12;  // M
static const int PROB_DOWN = 2;      // D
static const int PROB_UP = 1;        // U
static const int PROB_HEAL = 8;      // +
static const int PROB_CAMP = 4;      // ~

Map* create_map(int width, int height) {
    if (width < MAP_MIN) width = MAP_MIN;
    if (height < MAP_MIN) height = MAP_MIN;
    if (width > MAP_MAX) width = MAP_MAX;
    if (height > MAP_MAX) height = MAP_MAX;

    Map *m = malloc(sizeof(Map));
    if (! m) return NULL;
    m->width = width;
    m->height = height;
    m->data = malloc((size_t)width * height);
    if (!m->data) {
        free(m);
        return NULL;
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
                map_set(m, x, y, '#');
            } else map_set(m, x, y, 'X');
        }
    }
    return m;
}

void free_map(Map **m) {
    if (!m || ! *m) return;
    Map *mm = *m;
    if (mm->data) {
        print("память по адресу %p освобождена (map->data)", (void*)mm->data);
        free(mm->data);
    }
    print("память по адресу %p освобождена (Map struct)", (void*)mm);
    free(mm);
    *m = NULL;
}

char map_get(const Map *m, int x, int y) {
    if (!m) return 0;
    if (x < 0 || y < 0 || x >= m->width || y >= m->height) return '#';
    return m->data[y * m->width + x];
}

void map_set(Map *m, int x, int y, char v) {
    if (!m) return;
    if (x < 0 || y < 0 || x >= m->width || y >= m->height) return;
    m->data[y * m->width + x] = v;
}

void draw_map(const Map *m, int heroX, int heroY) {
    if (!m) return;
    for (int y = 0; y < m->height; ++y) {
        char line[1024];
        int idx = 0;
        for (int x = 0; x < m->width; ++x) {
            char c = map_get(m, x, y);
            if (x == heroX && y == heroY) {
                line[idx++] = '\x03';
                line[idx++] = ' ';
            } else {
                line[idx++] = c;
                line[idx++] = ' ';
            }
        }
        line[idx] = '\0';
        print("%s", line);
    }
}

void populate_map(Map *m, int depth) {
    if (!m) return;
    for (int y = 1; y < m->height - 1; ++y) {
        for (int x = 1; x < m->width - 1; ++x) {
            int r = random(1, 100);
            int pos = 0;
            pos += PROB_MONSTER;
            if (r <= pos) {
                map_set(m, x, y, 'M');
                continue;
            }
            pos += PROB_DOWN;
            if (r <= pos) {
                map_set(m, x, y, 'D');
                continue;
            }
            pos += PROB_UP;
            if (r <= pos) {
                map_set(m, x, y, 'U');
                continue;
            }
            pos += PROB_HEAL;
            if (r <= pos) {
                map_set(m, x, y, '+');
                continue;
            }
            pos += PROB_CAMP;
            if (r <= pos) {
                map_set(m, x, y, '~');
                continue;
            }
            map_set(m, x, y, 'X');
        }
    }

    if (depth == 1) {
        for (int tries = 0; tries < 200; ++tries) {
            int rx = random(1, m->width - 2);
            int ry = random(1, m->height - 2);
            if (map_get(m, rx, ry) == 'X') {
                map_set(m, rx, ry, 'U');
                break;
            }
        }
    }
}