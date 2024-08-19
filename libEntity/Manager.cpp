#include "Manager.h"

Manager *Manager::getInstance()
{
    static Manager* ins = nullptr;
    if(ins==nullptr) ins = new Manager();
    return ins;
}

void Manager::addEntity(Entity *e)
{
    entities[e->getType()].insert(e);
}

void Manager::update()
{
    for(int i=0; i<ENTITY_TYPR_NUM; ++i)
    {
        for (auto j:entities[i])
        {
            j->update();
        }
    }
}

void Manager::draw(QPainter *painter)
{
    for(int i=0; i<ENTITY_TYPR_NUM; ++i)
    {
        for (auto j:entities[i])
        {
            j->draw(painter);
        }
    }
    
}
