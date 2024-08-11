#ifndef ENTITY_H
#define ENTITY_H

#include <QPainter>

enum EntityType
{
    ET_none, ET_player, ET_bullet, ET_enemy
};

enum EntityState
{
    ES_alive, ES_destroy, ES_clean
};

class Entity
{
public:
    Entity() {};
    virtual ~Entity() {};
    virtual void update() = 0;
    virtual void draw(QPainter* painter) = 0;

    // the Acitve state of the Entity
    inline EntityState getState() { return state; }
    inline void destroy() { state=ES_destroy; }
    
    // the type of the Entity
    inline int type() { return m_type; }
    inline void setType(EntityType type) { this->m_type = type; }
protected:
    EntityState state = ES_alive;
    EntityType m_type = ET_none;
};

#endif