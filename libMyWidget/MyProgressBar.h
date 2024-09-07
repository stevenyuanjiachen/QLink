#ifndef MYPROGRESSBAR_H
#define MYPROGRESSBAR_H

#include <QObject>
#include <QPixmap>
#include <QRectF>
#include <QFontDataBase>
#include <QPainter>
#include <QTimer>

const int PROGRESS_BAR_FPS = 60;

class MyProgressBar : public QObject
{
public:
    MyProgressBar(int x, int y ,double time);
    ~MyProgressBar(){};
    void draw(QPainter* painter) ;
    void update() ;
public:
    void setTime(double time);
    void setValue(double value);
private:
    int x, y;
    int time, value;
    QTimer timer;

    // the frame
    QPixmap frame;
    
    // the bar
    int barX, barY, barWidth, barHeight;
    QRectF barRect;
    QPen barPen;
    QBrush barBrush;

    // the text
    int textX, textY, textWidth, textHeight;
    QString timeText;
    QPen textPen;
    QFont textFont;
};
#endif