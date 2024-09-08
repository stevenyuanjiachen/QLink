#include "Shuffle.h"
#include "EntityResList.h"


Shuffle::Shuffle(int x, int y):
    Item(ET_shuffle, x, y)
{
    pixmap.load(SHUFFLE_IMAGE);
}

void Shuffle::pickUp()
{
    emit signalAddTime(1);
    Item::pickUp();
}
