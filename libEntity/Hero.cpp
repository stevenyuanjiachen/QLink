#include "Hero.h"
#include "Map.h"

Hero::Hero(double x, double y)
: Sprite(x, y, STAND_DOWN_LIST[0]), state(HS_stand_down)
{
    this->setType(ET_hero);
    anima.append(Animation(STAND_UP_LIST));
    anima.append(Animation(STAND_DOWN_LIST));
    anima.append(Animation(STAND_LEFT_LIST));
    anima.append(Animation(STAND_RIGHT_LIST));
    anima.append(Animation(MOVE_UP_LIST));
    anima.append(Animation(MOVE_DOWN_LIST));
    anima.append(Animation(MOVE_LEFT_LIST));
    anima.append(Animation(MOVE_RIGHT_LIST));
}

void Hero::update()
{
    Sprite::update();
    anima[state].update();
    
    // limit the position of the Hero
    // out from left
    if(position.x() < 0)
    {
        position.setX(0);
    }
    // out from right
    if(position.x() > CUBE_LENGTH*MAP_WIDTH-pixmap.width())
    {
        position.setX(CUBE_LENGTH*MAP_WIDTH-pixmap.width());
    }
    // out from up
    if(position.y() < 0)
    {
        position.setY(0);
    }
    // out from down
    if(position.y() > CUBE_LENGTH*MAP_HEIGHT-pixmap.height())
    {
        position.setY(CUBE_LENGTH*MAP_HEIGHT-pixmap.height());
    }
}

void Hero::draw(QPainter *painter)
{
    anima[state].draw(painter, position);
}
