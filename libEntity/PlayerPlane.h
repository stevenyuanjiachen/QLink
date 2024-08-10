#ifndef PLAYERPLANE_H
#define PLAYERPLANE_H

#include "Sprite.h"
#include "Manager.h"
#include "Bullet.h"

class PlayerPlane : public Sprite
{
public:
    using Sprite::Sprite;
    void fire();
};

#endif