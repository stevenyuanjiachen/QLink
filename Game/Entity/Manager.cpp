#include "Manager.h"

template<class T>
T* Manager::addEntity(T* e)
{
    entities.insert(e);
    return e;
}

void Manager::update()
{
    for(auto& e : entities)
    {
        e->update();
    }
}

void Manager::draw(QPainter *painter)
{
    for(auto& e : entities)
    {
        e->draw(painter);
    }
}
