#ifndef MANAGER_H
#define MANAGER_H

#include "Entity.h"
#include <QSet>
#define Mgr Manager::getInstance()

// set the Manager as the instance
class Manager
{
public:
    // get the Manager Instance
    static Manager* getInstance()
    {
        static Manager* ins = nullptr;
        if(ins == nullptr)
            ins = new Manager();
        return ins;
    }
    // Add Entity
    template<class T>
    T* addEntity(T* e)
    {
        entities.insert(e);
        return e;
    }
    // draw and update
    void update()
    {
        for(auto& e : entities)
        {
            e->update();
        }
    }
    void draw(QPainter* painter)
    {
        for(auto& e : entities)
        {
            e->draw(painter);
        }
    }
    // refresh
    void refresh()
    {
        entities.removeIf([](Entity* e){
            if(e->isActive()==0)
            {
                delete e;
                return true;
            }else return false;
        });
    }
private:
    QSet<Entity*> entities;
    Manager(){}; 
};

#endif