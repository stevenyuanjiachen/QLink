#include "Hero.h"

Hero::Hero(double x, double y)
: Sprite(x, y, stand_down_list[0]), state(HS_stand_down)
{
    anima.append(Animation(stand_up_list));
    anima.append(Animation(stand_down_list));
    anima.append(Animation(stand_left_list));
    anima.append(Animation(stand_right_list));
    anima.append(Animation(move_up_list));
    anima.append(Animation(move_down_list));
    anima.append(Animation(move_left_list));
    anima.append(Animation(move_right_list));
}