#ifndef MYPROGRESSBAR_H
#define MYPROGRESSBAR_H

#include <QWidget>
#include <QPainter>

class MyProgressBar : public QWidget
{
public:
    MyProgressBar(int x, int y);
    ~MyProgressBar(){};

    void draw(QPainter* painter) ;
    void update() ;
private:
    int x, y;
    int width, height;
};
#endif