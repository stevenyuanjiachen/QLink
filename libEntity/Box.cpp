#include "Box.h"

Box::Box(int x, int y, BoxColor color):
    Entity(ET_box), state(BS_normal), 
    position(x, y), color(color), pixmap(BOX_RED_LIST[0])
{
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
        this->disconnect();
        break;
    case BS_triggered:
        emitSignalElimate();
        emitSignalCancelTrigger();
        break;
    case BS_triggering:
        if(!anima->isLastFrame()) anima->update();
        else state = BS_triggered;
        break;
    case BS_cancel_triggering:
        if(!anima->isFirstFrame()) anima->updateBack();
        else state = BS_normal;
        break;
    case BS_elimating:
        if(!anima->isLastFrame()) anima->update();
        else state = BS_elimate;
        break;
    case BS_elimate:
        state = BS_clean;
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

void Box::setPosition(int x, int y)
{
    position.setX(x);
    position.setY(y);
}

void Box::setMatrixPosition(int r, int c)
{
    matrixPosition.setX(r);
    matrixPosition.setY(c);
}

void Box::trigger()
{
    if(state == BS_normal) state = BS_triggering;
}

void Box::cancelTrigger()
{
    if(state == BS_triggered) state = BS_cancel_triggering;
}

void Box::emitSignalCancelTrigger()
{
    emit signalCancelTrigger(this);
}

void Box::elimate()
{
    state = BS_elimating;
}

void Box::emitSignalElimate()
{
    emit signalElimate(this);
}
