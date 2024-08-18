#include "Map.h"

Map::Map()
{
    for(int i=0; i<MAP_HEIGHT; ++i)
    {
        for(int j=0; j<MAP_WIDTH; ++j)
        {
            gameMap[i][j].load(map_list[map_num[i][j]]);
        }
    }
}

void Map::draw(QPainter *painter)
{
    for(int i=0; i<MAP_HEIGHT; ++i)
    {
        for(int j=0; j<MAP_WIDTH; ++j)
        {
            painter->drawPixmap(j*CUBE_LENGTH, i*CUBE_LENGTH, gameMap[i][j]);
        }
    }
}

void Map::update()
{
}
