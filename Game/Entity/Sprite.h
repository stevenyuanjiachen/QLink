#ifndef SPRITE_H
#define SPRITE_H

#include "Entity.h"
#include <QVector2D>

class Sprite : public Entity
{
public:
    Sprite();
    Sprite(double x, double y, const QString& image);

    void update() override;
    void draw(QPainter* painter) override;
    void moveBy(int dx, int dy);
private:
    QVector2D position;
    QPixmap pixmap; // image
    // 速度
    QVector2D velocity;
    int speed;
};




#endif