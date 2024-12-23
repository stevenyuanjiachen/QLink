#include "Manager.h"

Manager *Manager::getInstance()
{
    static Manager *ins = nullptr;
    if (ins == nullptr) ins = new Manager();
    return ins;
}

void Manager::addEntity(Entity *e)
{
    entities[e->getType()].insert(e);
}

QSet<Entity *> &Manager::getEntity(EntityType type)
{
    return entities[type];
}

void Manager::update()
{
    this->refresh();
    for (int i = 0; i < ENTITY_TYPE_NUM; ++i) {
        for (auto j : entities[i]) {
            j->update();
        }
    }
}

void Manager::draw(QPainter *painter)
{
    for (int i = 0; i < ENTITY_TYPE_NUM; ++i) {
        for (auto j : entities[i]) {
            j->draw(painter);
        }
    }
}

void Manager::refresh()
{
    for (int i = 0; i < ENTITY_TYPE_NUM; ++i) {
        entities[i].removeIf([](Entity *e) {
            if (e->isActive()) {
                return false;
            } else {
                delete e;
                return true;
            }
        });
    }
}

void Manager::clean()
{
    for (int i = 0; i < ENTITY_TYPE_NUM; ++i) {
        for (auto j : entities[i]) {
            j->destroy();
        }
    }
    refresh();
}

void Manager::cleanEntityType(EntityType type)
{
    for (auto i : entities[type]) {
        i->destroy();
    }

    refresh();
}
