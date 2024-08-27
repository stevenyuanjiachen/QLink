#ifndef BOX_H
#define BOX_H

#include "Entity.h"
#include "Animation.h"
#include "ResourceList.h"
#include <QVector2D>
#include <QElapsedTimer>
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
    BS_cancel_triggered,
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
    void collideEvent();
    void cancelTriggered() { state = BS_cancel_triggered; };
    BoxColor getColor() { return color; }
    void elimate() { state = BS_elimate; }
private:
    QVector2D position;
    QPixmap pixmap;
    QRect collider;
    Animation *anima;
    BoxColor color;
    BoxState state;
    QElapsedTimer triggeredTimer;
};

#endif