#include "Bullet.h"

void Bullet::update()
{
    Sprite::update();
    if(this->position.y()<-5){
        this->state = ES_clean;
    }
}