#include "PlayerPlane.h"

void PlayerPlane::fire()
{
    Sprite* bullet = new Sprite(
        this->position.x()+this->width()/2,
        this->position.y()-10,
        "../images/bullet1.png");
    bullet->velocity.setY(-1);
    Mgr->addEntity(bullet);
}
