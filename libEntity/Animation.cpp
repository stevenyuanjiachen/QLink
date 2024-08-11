#include "Animation.h"

Animation::Animation(int speed)
    : images_num(0), m_speed(speed)
{
    elapsedTimer.start();
    index = 0;
}

void Animation::addAnima(const QString &image)
{
    images[images_num++].load(image);
}

void Animation::update()
{
    index = elapsedTimer.elapsed()/m_speed % images_num;
}

bool Animation::updateOnce()
{
    m_speed = 5;
    ++counter;
    if(counter%m_speed==0) ++index;
    if(counter==m_speed*this->images_num) return true;
    return false;
}

void Animation::draw(QPainter *painter, QVector2D position)
{
    if(images_num==0)
        throw "Animation has no image!";
    painter->drawPixmap(position.toPoint(), images[index]);
}
