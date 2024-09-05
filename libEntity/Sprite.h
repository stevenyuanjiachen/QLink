#ifndef SPRITE_H
#define SPRITE_H

#include "Entity.h"
#include <QVector2D>
#include <QRect>

class Sprite : public Entity
{
    Q_OBJECT
public:
    Sprite(double x, double y, const QString& image);

    void update() override;
    void draw(QPainter* painter) override;
public:
    QVector2D position;
    QPixmap pixmap; // image
    // 速度
    QVector2D velocity;
    int speed;
    //碰撞箱
    QRect collider;
};




#endif