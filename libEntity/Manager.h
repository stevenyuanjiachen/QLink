#ifndef MANAGER_H
#define MANAGER_H

#include "Entity.h"
#include <QSet>
#include <QPainter>
#define Mgr Manager::getInstance()

class Manager
{
public:
    static Manager* getInstance();
    void addEntity(Entity* e);

    void update();
    void draw(QPainter* painter);
    void refresh();
    
private:
    Manager(){}
    QSet<Entity*> entities[ENTITY_TYPR_NUM];
};
#endif