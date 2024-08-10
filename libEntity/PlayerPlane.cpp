#include "PlayerPlane.h"

PlayerPlane::PlayerPlane()
{
}

void PlayerPlane::fire(Manager& mgr)
{
    mgr.addEntity(new Sprite);
}
