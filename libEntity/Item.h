#pragma once

#include "Entity.h"
#include <QPixmap>
#include <QPainter>
#include <QRect>

class Item : public Entity
{
public:
    Item(EntityType type, int x = 0, int y = 0);
    void update() override;
    void draw(QPainter *painter) override;
    QRect &getCollider() { return collider; }
    int width() { return pixmap.width(); }
    int height() { return pixmap.height(); }
    void setPosition(int x, int y);
    virtual void pickUp();

protected:
    int x, y;
    QPixmap pixmap;
    QRect collider;
};