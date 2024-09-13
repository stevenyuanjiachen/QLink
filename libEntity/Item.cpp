#include "Item.h"
#include "EntityResList.h"

Item::Item(ItemType itemType, int x, int y):
    Entity(ET_item),itemType(itemType), x(x), y(y)
{
    switch (itemType)
    {
    case IT_add1s:
        pixmap.load(ADD1S_IMAGE);
        break;
    case IT_flash:
        pixmap.load(FLASH_IMAGE);
        break;
    case IT_shuffle:
        pixmap.load(SHUFFLE_IMAGE);
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
