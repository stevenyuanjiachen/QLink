#ifndef MANAGER_H
#define MANAGER_H

#include "Entity.h"
#include <QSet>

class Manager
{
public:
    Manager(){};
    template<class T>
    T* addEntity(T* e);
    void update();
    void draw(QPainter* painter);
private:
    QSet<Entity*> entities;
};



#endif