#ifndef PLAYERPLANE_H
#define PLAYERPLANE_H

#include "Sprite.h"
#include "Manager.h"
#include "Bullet.h"

class PlayerPlane : public Sprite
{
public:
    PlayerPlane(double x, double y, const QString& image);
    void destroy() ;
    void fire() ;

};

#endif