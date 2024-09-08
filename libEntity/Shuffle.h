#pragma once

#include "Entity.h"
#include <QPixmap>
#include <QPainter>

class  Shuffle: public Entity
{
    Q_OBJECT
public:
    Shuffle(int x, int y);
    ~Shuffle();
    void update() override;
    void draw(QPainter* painter);
    void pickUp();

signals:
    void signalAddTime(int time = 1);

private:
    int x, y;
    QPixmap pixmap;
};