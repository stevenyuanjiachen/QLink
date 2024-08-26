#include "Hero.h"
#include "Map.h"

Hero::Hero(double x, double y)
    : Sprite(x, y, STAND_DOWN_LIST[0]), 
    state(HS_stand_down), blockState(block_none)
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
    // Reset the collider
    collider = QRect(position.x(), position.y()+30, pixmap.width(), pixmap.height()-30);
    
    // edge block
    if(this->position.y()<0) blockState = block_up;
    if(this->position.y()>MAP_HEIGHT*CUBE_LENGTH-pixmap.height()) blockState = block_down;
    if(this->position.x()<0) blockState = block_left;
    if(this->position.x()>MAP_WIDTH*CUBE_LENGTH-pixmap.width()) blockState = block_right;

    // limit the position of the Hero
    switch (blockState)
    {
    case block_up:
        position.setY(position.y()+speed);
        blockState = block_none;
        break;
    case block_down:
        position.setY(position.y()-speed);
        blockState = block_none;
        break;
    case block_left:
        position.setX(position.x()+speed);
        blockState = block_none;
        break;
    case block_right:
        position.setX(position.x()-speed);
        blockState = block_none;
        break;
    }
}

void Hero::draw(QPainter *painter)
{
    anima[state].draw(painter, position);
}

void Hero::collideBoxEvent(Box* box)
{
    // limit the position
    switch (state)
    {
    case HS_move_up:
        blockState = block_up;
        break;
    case HS_move_down:
        blockState = block_down;
        break;
    case HS_move_left:
        blockState = block_left;
        break;
    case HS_move_right:
        blockState = block_right;
        break;
    }

}
