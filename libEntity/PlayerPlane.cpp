#include "PlayerPlane.h"

PlayerPlane::PlayerPlane(double x, double y, const QString &image)
    : Sprite(x, y, image)
{
    animas[AT_normal].addAnima("../images/me2.png");
    animas[AT_destroy].addAnima("../images/me_destroy_1.png");
    animas[AT_destroy].addAnima("../images/me_destroy_2.png");
    animas[AT_destroy].addAnima("../images/me_destroy_3.png");
    animas[AT_destroy].addAnima("../images/me_destroy_4.png");
}

void PlayerPlane::fire()
{
    Bullet* bullet = new Bullet(0,0,"../images/bullet1.png");
    bullet->setType(ET_bullet);
    bullet->position.setX(this->position.x()+this->width()/2);
    bullet->position.setY(this->position.y()-bullet->height());
    bullet->velocity.setY(-1);
    Mgr->addEntity(bullet);
}
