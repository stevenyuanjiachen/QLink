#pragma once

#include "Entity.h"
#include <QSet>
#include <QPainter>
#define Mgr Manager::getInstance()

class Manager
{
public:
    static Manager* getInstance();
    void addEntity(Entity* e);
    QSet<Entity*>& getEntity(EntityType type);

    void update();
    void draw(QPainter* painter);
    void refresh();
    void clean();
    
private:
    Manager(){}
    QSet<Entity*> entities[ENTITY_TYPE_NUM];
};