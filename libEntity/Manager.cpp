#include "Manager.h"

Manager *Manager::getInstance()
{
    static Manager *ins = nullptr;
    if (ins == nullptr)
        ins = new Manager();
    return ins;
}

Entity *Manager::addEntity(Entity *e)
{
    entities[e->type()].insert(e);
    return e;
}

void Manager::update()
{
    for (int i = 0; i < 10; ++i)
        for (auto &e : entities[i])
            e->update();
}

void Manager::draw(QPainter *painter)
{
    for (int i = 0; i < 10; ++i)
        for (auto &e : entities[i])
            e->draw(painter);
}

void Manager::refresh()
{
    for (int i = 0; i < 10; ++i)
    {
        entities[i].removeIf([](Entity *e)
                             {
            if(e->isActive()==0)
            {
                delete e;
                return true;
            }else return false; });
    }
}