#pragma once

#include "Entity.h"
#include <QPixmap>
#include <QPainter>
#include <QRect>

const int ITEM_TYPE_NUM = 2;
enum ItemType {
    IT_shuffle,
    IT_flash
};

class Item : public Entity
{
public:
    Item(ItemType type, int x = 0, int y = 0);
    void update() override;
    void draw(QPainter *painter) override;

    QRect &getCollider() { return collider; }
    int width() { return pixmap.width(); }
    int height() { return pixmap.height(); }
    void setPosition(int x, int y);
    int getX() const { return x; }
    int getY() const { return y; }
    virtual void pickUp();
    ItemType getItemType() const { return itemType; }

protected:
    ItemType itemType;
    int x, y;
    QPixmap pixmap;
    QRect collider;
};