#include "Map.h"

Map::Map()
{
    for(int i=0; i<BACKGROUND_HRIGHT; ++i)
    {
        for(int j=0; j<BACKGROUND_WIDTH; ++j)
        {
            gameMap[i][j].load(MAP_LIST[background[i][j]]);
        }
    }
}

void Map::draw(QPainter *painter)
{
    for(int i=0; i<BACKGROUND_HRIGHT; ++i)
    {
        for(int j=0; j<BACKGROUND_WIDTH; ++j)
        {
            painter->drawPixmap(j*CUBE_LENGTH, i*CUBE_LENGTH, gameMap[i][j]);
        }
    }
}

void Map::update()
{
}
