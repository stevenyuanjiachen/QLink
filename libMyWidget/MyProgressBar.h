#pragma once

#include <QObject>
#include <QPixmap>
#include <QRectF>
#include <QFontDataBase>
#include <QPainter>
#include <QTimer>

const int MY_PROGRESS_BAR_WIDTH = 346;
const int MY_PROGRESS_BAR_HEIGHT = 36;

class MyProgressBar : public QObject
{
    Q_OBJECT
public:
    MyProgressBar(int x, int y ,double time);
    ~MyProgressBar(){};
    void draw(QPainter* painter) ;
    void update() ;
    void setTime(double time);
    void setValue(double value);
    void addTime(int time);
    void pauseBar();
    void contineBar();

signals:
    void signalEnd();

private:
    bool pause;
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