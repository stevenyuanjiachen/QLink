#ifndef PLAYERPLANE_H
#define PLAYERPLANE_H

#include "Sprite.h"
#include "Manager.h"

class PlayerPlane : public Sprite
{
public:
    PlayerPlane();
    void fire();
};

#endif