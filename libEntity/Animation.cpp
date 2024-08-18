#include "Animation.h"
#include "Map.h"

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
    painter->drawPixmap(postion.x()-CUBE_LENGTH*MAP_WIDTH, postion.y(), images[index]); // left
    painter->drawPixmap(postion.x()+CUBE_LENGTH*MAP_WIDTH, postion.y(), images[index]); // right
    painter->drawPixmap(postion.x(), postion.y()-CUBE_LENGTH*MAP_HEIGHT, images[index]); // up
    painter->drawPixmap(postion.x(), postion.y()+CUBE_LENGTH*MAP_HEIGHT, images[index]); // down

}

void Animation::restart()
{
    counter = 0;
    index = 0;
}
