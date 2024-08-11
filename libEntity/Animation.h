#ifndef ANIMATION_H
#define ANIMATION_H

#include <QPainter>
#include <QElapsedTimer>
#include <QVector2D>

const int ANIMA_IMAGES_NUM = 10;
const int ANIMA_TYPE_COUNT = 2;

enum AnimaType{
    AT_normal,
    AT_destroy
};

class Animation
{
public:
    Animation(int speed = 100);
    void addAnima(const QString& image);

    void update();
    bool updateOnce();
    void draw(QPainter* painter, QVector2D position);
private:
    QPixmap images[ANIMA_IMAGES_NUM];
    int images_num;

    QElapsedTimer elapsedTimer;
    int m_speed;
    int index;
};

#endif