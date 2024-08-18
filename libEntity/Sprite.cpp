#include "Sprite.h"

Sprite::Sprite(double x, double y, const QString &image):
    position(x, y), speed(3), velocity(0,0)
{
    pixmap.load(image);
}

void Sprite::update()
{
    this->position += speed * velocity;
    collider = QRect(position.toPoint(), pixmap.size());
}

void Sprite::draw(QPainter *painter)
{
    painter->drawPixmap(position.toPoint(), pixmap);
}

