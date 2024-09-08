#include "Item.h"
#include "EntityResList.h"

Item::Item(EntityType type, int x, int y):
    Entity(type), x(x), y(y)
{
    
}

void Item::update()
{
    
}

void Item::draw(QPainter *painter)
{
    painter->drawPixmap(x, y, pixmap);
}

void Item::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Item::pickUp()
{
    this->destroy();
}
