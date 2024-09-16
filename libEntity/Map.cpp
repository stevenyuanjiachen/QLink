#include "Map.h"

Map::Map()
{
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            gameMap[i][j].load(MAP_LIST[map[i][j]]);
        }
    }
}

void Map::draw(QPainter *painter)
{
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            painter->drawPixmap(j * CUBE_LENGTH, i * CUBE_LENGTH, gameMap[i][j]);
        }
    }
}

