#ifndef MANAGER_H
#define MANAGER_H

#include "Entity.h"
#include <QSet>

class Manager
{
public:
    Manager(){};
    Entity* addEntity(Entity* e);
    void update();
    void draw(QPainter* painter);
private:
    QSet<Entity*> entities;
};



#endif