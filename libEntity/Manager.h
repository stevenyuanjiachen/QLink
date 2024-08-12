#ifndef MANAGER_H
#define MANAGER_H

#include "Entity.h"

class Manager
{
public:
    void addEntity(Entity* e);
    
private:
    Manager(){}
    QSet<Entity*> entities[ENTITY_TYPR_NUM];
};
#endif