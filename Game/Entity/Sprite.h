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
    void draw(QPainter* painter);
private:
    QVector2D position;
    QPixmap pixmap; // image
};




#endif