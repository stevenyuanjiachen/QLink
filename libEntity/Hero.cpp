#include "Hero.h"
#include "Map.h"

Hero::Hero(double x, double y)
: Sprite(x, y, stand_down_list[0]), state(HS_stand_down)
{
    this->set_type(ET_hero);
    anima.append(Animation(stand_up_list));
    anima.append(Animation(stand_down_list));
    anima.append(Animation(stand_left_list));
    anima.append(Animation(stand_right_list));
    anima.append(Animation(move_up_list));
    anima.append(Animation(move_down_list));
    anima.append(Animation(move_left_list));
    anima.append(Animation(move_right_list));
}

void Hero::update()
{
    Sprite::update();
    anima[state].update();
    
    // limit the position of the Hero
    // out from left
    if(position.x() < -pixmap.width())
    {
        position.setX(CUBE_LENGTH*MAP_WIDTH-pixmap.width());
    }
    // out from right
    if(position.x() > CUBE_LENGTH*MAP_WIDTH)
    {
        position.setX(0);
    }
    // out from up
    if(position.y() < -pixmap.height())
    {
        position.setY(CUBE_LENGTH*MAP_HEIGHT-pixmap.height());
    }
    // out from down
    if(position.y() > CUBE_LENGTH*MAP_HEIGHT)
    {
        position.setY(0);
    }
}

void Hero::draw(QPainter *painter)
{
    anima[state].draw(painter, position);
}
