#include "Item.h"
#include "EntityResList.h"

Item::Item(ItemType itemType, int x, int y):
    Entity(ET_item),itemType(itemType), x(x), y(y)
{
    switch (itemType)
    {
    case IT_shuffle:
        pixmap.load(SHUFFLE_IMAGE);
        break;
    case IT_hint:
        pixmap.load(HINT_IMAGE);
        break;
    }
}

void Item::update()
{
    collider.setRect(x, y, pixmap.width(), pixmap.height());
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
