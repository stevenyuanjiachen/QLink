#pragma once

#include <QPainter>
#include <QSet>

#include "Entity.h"
#define Mgr Manager::getInstance()

class Manager {
public:
    static Manager* getInstance();
    void addEntity(Entity* e);
    QSet<Entity*>& getEntity(EntityType type);

    void update();
    void draw(QPainter* painter);
    void refresh();
    void clean();
    void cleanEntityType(EntityType type);

private:
    Manager() {}
    QSet<Entity*> entities[ENTITY_TYPE_NUM];
};