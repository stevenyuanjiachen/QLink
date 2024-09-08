#include "Shuffle.h"
#include "EntityResList.h"

Shuffle::Shuffle(int x, int y):
    Entity(ET_item), x(x), y(y)
{
    pixmap.load(SHUFFLE_IMAGE);
}

Shuffle::~Shuffle()
{

}

void Shuffle::update()
{
    
}

void Shuffle::draw(QPainter *painter)
{
    painter->drawPixmap(x, y, pixmap);
}

void Shuffle::pickUp()
{
    emit signalAddTime(1);
    this->destroy();
}
