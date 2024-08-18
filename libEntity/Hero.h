#ifndef HERO_H
#define HERO_H

#include "Sprite.h"
#include "Animation.h"
#include "HeroResourceList.h"

const int HERO_STATE_NUM = 8;
enum HeroState
{
    HS_stand_up, HS_stand_down, HS_stand_left, HS_stand_right, 
    HS_move_up, HS_move_down, HS_move_left, HS_move_right,
};

class Hero: public Sprite
{
public:
    Hero(double x, double y);

private:
    HeroState state;
    QVector<Animation> anima;
};


#endif