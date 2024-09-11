#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>
#include <QPainter>

const int ENTITY_TYPE_NUM = 5;
enum EntityType{
    ET_none, ET_box, ET_shuffle, ET_flash, ET_hero
};

class Entity: public QObject
{
    Q_OBJECT
public:
    Entity(EntityType type = ET_none);
    virtual ~Entity();
    virtual void update() = 0;
    virtual void draw(QPainter* painter) = 0;

    // the Acitve state of the Entity
    inline bool isActive() { return this->active; }
    inline void destroy() { this->active = false; }
    
    // the type of the Entity
    inline EntityType getType() { return type; }
    inline void setType(EntityType type) { this->type = type; }
protected:
    bool active = true;
    EntityType type = ET_none;
};

#endif