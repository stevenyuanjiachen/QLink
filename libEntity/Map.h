#ifndef MAP_H
#define MAP_H

#include "Entity.h"

class Map : public Entity
{
public:
    Map(double width, double height);
    void draw(QPainter* painter);
    void update();
    inline double width(){ return WIDTH; }
    inline double height(){ return HEIGHT; }
private:
    QPixmap pixmap;
    double ypos1, ypos2;
    double speed ;
    const double WIDTH, HEIGHT;
};



#endif