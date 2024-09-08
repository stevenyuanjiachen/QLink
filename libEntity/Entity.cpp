#include "Entity.h"


Entity::Entity(EntityType type):
    QObject(nullptr), type(type)
{
    
}

Entity::~Entity()
{
}
