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
    int frameWidth; 
    // the frame
    QRectF frame1Rect, frame2Rect; 
    QPen frame1Pen, frame2Pen; 
    QBrush frame1Brush, frame2Brush;
    // the bar
    QRectF barRect;
    QPen barPen;
    QBrush barBrush;

    int time, value;
    QTimer timer;
};
#endif