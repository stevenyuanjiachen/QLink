#pragma once

#include <QPainter>
#include <QPixmap>
#include <QVector2D>

const int ANIMA_MAX_IMAGE_NUM = 10;

class Animation {
public:
    Animation(const QStringList& image_list);

    void update();
    void updateBack();
    void draw(QPainter* painter, const QVector2D& postion);
    void restart();
    bool isLastFrame();  // whether is the last frame
    bool isFirstFrame(); // whether is the first frame

private:
    QPixmap images[ANIMA_MAX_IMAGE_NUM];
    int image_num;

    int index;
    int animation_speed;
    int counter;
};