#ifndef ENTITY_H
#define ENTITY_H

#include <QPainter>
class Entity
{
public:
    Entity() {};
    virtual void update() = 0;
    virtual void draw(QPainter* painter) = 0;

    // the Acitve state of the Entity
    inline bool isActive() { return this->active; }
    inline void destroy() { this->active = false; }
    
    // the type of the Entity
    inline int type() { return m_type; }
    inline void set_type(int type) { this->m_type = type; }
private:
    bool active = true;
    int m_type = 0;
};

#endif