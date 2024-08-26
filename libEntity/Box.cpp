#include "Box.h"

Box::Box(int x, int y, BoxColor color):
    position(x, y), color(color), state(BS_normal), pixmap(BOX_RED_LIST[0])
{
    this->setType(ET_box);
    // load the animation
    switch (color)
    {
    case BC_red:
        anima = new Animation(BOX_RED_LIST);
        break;
    case BC_blue:
        anima = new Animation(BOX_BLUE_LIST);
        break;
    case BC_pink:
        anima = new Animation(BOX_PINK_LIST);
        break;
    }
}

void Box::update()
{
    collider = QRect(position.toPoint(), pixmap.size());
    switch (state)
    {
    case BS_triggered:
        if(!anima->isLastFrame()) anima->update();
        break;
    case BS_clean:
        this->destroy();
        break;
    }
}

void Box::draw(QPainter *painter)
{
    anima->draw(painter, position);
}
