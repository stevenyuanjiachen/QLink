#include "PlayerPlane.h"

void PlayerPlane::fire()
{
    Bullet* bullet = new Bullet(0,0,"../images/bullet1.png");
    bullet->position.setX(this->position.x()+this->width()/2);
    bullet->position.setY(this->position.y()-bullet->height());
    bullet->velocity.setY(-1);
    Mgr->addEntity(bullet);
}
