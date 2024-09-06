#ifndef MYPROGRESSBAR_H
#define MYPROGRESSBAR_H

#include <QObject>
#include <QRectF>
#include <QPainter>
#include <QTimer>

const int PROGRESS_BAR_FPS = 60;

class MyProgressBar : public QObject
{
public:
    MyProgressBar(int x, int y, int w, int h ,double time);
    ~MyProgressBar(){};
    void draw(QPainter* painter) ;
    void update() ;
public:
    void setTime(double time);
    void setValue(double value);
private:
    int x, y, width, height;
    int offset; // 进度条相对边框收缩
    // the frame
    QRectF frameRect; 
    QPen framePen; 
    QBrush frameBrush;
    // the bar
    QRectF barRect;
    QPen barPen;
    QBrush barBrush;

    int time, value;
    QTimer timer;
};
#endif