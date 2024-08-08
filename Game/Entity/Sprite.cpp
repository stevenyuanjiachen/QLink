#include "Sprite.h"

Sprite::Sprite(double x, double y, const QString &image):
    position(x, y), speed(3), velocity(1,1)
{
    pixmap.load(image);
}

void Sprite::update()
{
    
}

void Sprite::draw(QPainter *painter)
{
    painter->drawPixmap(position.toPoint(), pixmap);
}

void Sprite::moveBy(int dx, int dy)
{
    this->position += QVector2D(dx, dy);
}
