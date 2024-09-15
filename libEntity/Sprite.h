#pragma once

#include "Entity.h"
#include <QVector2D>
#include <QRect>

class Sprite : public Entity
{
    Q_OBJECT
public:
    Sprite(EntityType type, double x, double y, const QString& image);

    void update() override;
    void draw(QPainter* painter) override;
public:
    QVector2D position;
    QPixmap pixmap; // image
    QVector2D velocity; // 速度
    int speed;
    QRect collider; // 碰撞箱
};
