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
    static Manager *getInstance();
    // Add Entity
    Entity *addEntity(Entity *e);
    // draw and update
    void update();
    void draw(QPainter *painter);
    // refresh
    void refresh();
    QSet<Entity* > getEntities(EntityType type) { return entities[type]; }

private:
    QSet<Entity *> entities[10];
    Manager() {}
};

#endif