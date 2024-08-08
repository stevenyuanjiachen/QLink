#include "Sprite.h"

Sprite::Sprite(double x, double y, const QString &image):
    position(x, y)
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
