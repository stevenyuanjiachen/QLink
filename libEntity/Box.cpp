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

    // state machine
    switch (state)
    {
    case BS_normal:
        break;
    case BS_triggered:
        break;
    case BS_triggering:
        if(!anima->isLastFrame()) anima->update();
        else state = BS_triggered;
        break;
    case BS_cancel_triggering:
        if(!anima->isFirstFrame()) anima->updateBack();
        else state = BS_normal;
        break;
    case BS_elimate:
        if(!anima->isLastFrame()) anima->update();
        else state = BS_clean;
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

void Box::trigger()
{
    if(state == BS_normal) state = BS_triggering;
}

void Box::cancelTrigger()
{
    if(state == BS_triggered) state = BS_cancel_triggering;
}

void Box::elimate()
{
    state = BS_elimate;
}
