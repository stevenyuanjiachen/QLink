#include "Animation.h"

Animation::Animation(const QStringList& image_list):
    index(0), animation_speed(6), counter(0)
{
    image_num = image_list.count();
    for(int i=0; i<image_num; ++i)
    {
        images[i].load(image_list[i]);
    }
}

void Animation::update()
{
    if(counter==animation_speed)
    {
        ++index;
        counter=0;
    } 
    ++counter;
    index%=image_num;
}

void Animation::draw(QPainter *painter, const QVector2D& postion)
{
    painter->drawPixmap(postion.toPoint(), images[index]);
}

void Animation::restart()
{
    counter = 0;
    index = 0;
}
