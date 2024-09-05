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
    BC_red = 1,
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
    BS_elimating,
    BS_clean
};

class Box : public Entity
{
    Q_OBJECT
public:
    Box(int x, int y, BoxColor color);
    void update() override;
    void draw(QPainter *painter) override;

public:
    inline QRect &getCollider() { return collider; }
    inline BoxColor getColor() const { return color; }
    inline int getR() const { return matrixPosition.x(); }
    inline int getC() const { return matrixPosition.y(); }
    void setMatrixPosition(int r, int c);
    void trigger();
    void cancelTrigger();
    void emitSignalCancelTrigger();
    void elimate(); 
    void emitSignalElimate();

signals:
    void signalElimate(Box* box);
    void signalCancelTrigger(Box* box);

private:
    QVector2D position;
    QVector2D matrixPosition;
    QPixmap pixmap;
    QRect collider;
    Animation *anima;
    BoxColor color;
    BoxState state;
};

#endif