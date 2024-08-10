#include "EnemyPlane.h"

void EnemyPlane::update()
{
    Sprite::update();
    if(this->position.y()>700+5){
        this->active = false;
    }
}