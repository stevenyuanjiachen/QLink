#ifndef BOX_H
#define BOX_H

#include "Entity.h"
#include "Animation.h"
#include "ResourceList.h"
#include <QVector2D>
#include <QPixmap>
#include <QRect>
#include <QVector>

const int BOX_COLOR_NUM = 3;
enum BoxColor
{
    BC_red,
    BC_pink,
    BC_blue
};
enum BoxState
{
    BS_normal,
    BS_triggered,
    BS_triggering,
    BS_cancel_triggering,
    BS_elimate,
    BS_clean
};

class Box : public Entity
{
public:
    Box(int x, int y, BoxColor color);
    void update() override;
    void draw(QPainter *painter) override;

public:
    QRect &getCollider() { return collider; }
    BoxColor getColor() const { return color; }
    void trigger();
    void cancelTrigger();
    void elimate(); 

private:
    QVector2D position;
    QPixmap pixmap;
    QRect collider;
    Animation *anima;
    BoxColor color;
    BoxState state;
};

#endif