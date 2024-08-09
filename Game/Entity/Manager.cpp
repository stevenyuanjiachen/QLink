#include "Manager.h"

Entity* Manager::addEntity(Entity* e)
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
