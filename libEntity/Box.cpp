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
    // 
    triggeredTimer.start();
}

void Box::update()
{
    collider = QRect(position.toPoint(), pixmap.size());
    switch (state)
    {
    case BS_normal:
        break;
    case BS_triggered:
        if(!anima->isLastFrame()) anima->update();
        break;
    case BS_cancel_triggered:
        if(!anima->isFirstFrame()) anima->updateBack();
        break;
    case BS_elimate:
        if(!anima->isFirstFrame())
        {
            anima->updateBack();
            break;
        }else
        {
            state = BS_clean;
        }
    case BS_clean:
        this->destroy();
        break;
    }
}

void Box::draw(QPainter *painter)
{
    anima->draw(painter, position);
}

void Box::collideEvent()
{
    if(triggeredTimer.elapsed() < 500)
        return;
    switch (state)
    {
    case BS_normal:
        state = BS_triggered;
        triggeredTimer.restart();
        break;
    case BS_triggered:
        state = BS_cancel_triggered;
        triggeredTimer.restart();
        break;
    }
}
