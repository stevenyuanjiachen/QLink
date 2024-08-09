#include "Map.h"
// #include "Game.h"

Map::Map(double width, double height): 
    speed(2), ypos1(-700), ypos2(0),
    WIDTH(width), HEIGHT(height)
{   
    pixmap.load("../images/background.png");
}

void Map::draw(QPainter *painter)
{
    painter->drawPixmap(0, ypos1, pixmap);
    painter->drawPixmap(0, ypos2, pixmap);
}

void Map::update()
{
    ypos1+=speed;
    if(ypos1>=700) ypos1=-700;
    ypos2+=speed;
    if(ypos2>=700) ypos2=-700; 
}
