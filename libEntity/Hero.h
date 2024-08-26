#ifndef HERO_H
#define HERO_H

#include "Sprite.h"
#include "Animation.h"
#include "ResourceList.h"

const int HERO_STATE_NUM = 8;
enum HeroState
{
    HS_stand_up, HS_stand_down, HS_stand_left, HS_stand_right, 
    HS_move_up, HS_move_down, HS_move_left, HS_move_right,
    HS_collide_up, HS_collide_down, HS_collide_left, HS_collide_right
};

class Hero: public Sprite
{
    Q_OBJECT
public:
    Hero(double x, double y);
    void update() override;
    void draw(QPainter* painter) override;
public:
    void setState(HeroState state) { this->state = state; }
    HeroState getState() const {return this->state; }
    bool intersects(const QRect& coll) { return collider.intersects(coll); }
    void collideBoxEvent();
private:
    HeroState state;
    QVector<Animation> anima;
};

#endif