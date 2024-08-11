#ifndef SPRITE_H
#define SPRITE_H

#include "Entity.h"
#include "Animation.h"
#include <QVector2D>
#include <QRect>

class Sprite : public Entity
{
public:
    Sprite(double x, double y, const QString& image);

    void update() override;
    void draw(QPainter* painter) override;

    inline double width() { return pixmap.width(); }
    inline double height() { return pixmap.height(); }   
public:
    QVector2D position;
    // Animation
    QPixmap pixmap;
    Animation animas[ANIMA_TYPE_COUNT];
    // 速度
    QVector2D velocity;
    int speed;
    //碰撞箱
    QRect collider;
private:
    bool destroy_begin=false;
};


#endif