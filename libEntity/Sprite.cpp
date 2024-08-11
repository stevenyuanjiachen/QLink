#include "Sprite.h"

Sprite::Sprite(double x, double y, const QString &image):
    position(x, y), speed(3), velocity(0,0)
{
    pixmap.load(image);
    animas[AT_normal].addAnima(image);
    animas[AT_destroy].addAnima(image);
}

void Sprite::update()
{
    if(state==ES_alive)
    {
        this->position += speed * velocity;
        animas[AT_normal].update();
        collider = QRect(position.toPoint(), pixmap.size());
    }
    else if(state==ES_destroy)
    {
        if(animas[AT_destroy].updateOnce()) state=ES_clean;
    }
}

void Sprite::draw(QPainter *painter)
{
    if(state==ES_alive)
        animas[AT_normal].draw(painter, position);
    else if(state==ES_destroy)
        animas[AT_destroy].draw(painter, position);
}