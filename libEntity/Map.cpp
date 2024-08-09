#include "Map.h"
// #include "Game.h"

Map::Map()
{
    ypos1 = -700;
    ypos2 = 0;
    pixmap.load("../images/background.png");
}

void Map::draw(QPainter *painter)
{
    painter->drawPixmap(0, ypos1, pixmap);
    painter->drawPixmap(0, ypos2, pixmap);
}

void Map::update()
{
}
