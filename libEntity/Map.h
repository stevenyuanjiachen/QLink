#ifndef MAP_H
#define MAP_H

#include "Entity.h"

class Map : public Entity
{
public:
    Map(/* args */);
    void draw(QPainter* painter);
    void update();
private:
    QPixmap pixmap;
    int ypos1, ypos2;
};



#endif