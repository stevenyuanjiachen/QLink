#include "MyProgressBar.h"

MyProgressBar::MyProgressBar(int x, int y, int w, int h, double time):
    QObject(nullptr), x(x), y(y), width(w), height(h), frameWidth(4),
    time(time*1000), value(time*1000)
{
    // set the location
    frame1Rect.setRect(x, y, w, h);
    frame2Rect.setRect(x+frameWidth, y+frameWidth, w-2*frameWidth, h-2*frameWidth);
    barRect.setRect(x+frameWidth+2, y+frameWidth+2, w-2*frameWidth-4, h-2*frameWidth-4);


    // set the frame
    frame1Pen.setWidth(2);
    frame1Pen.setColor(QColor(160, 82, 45));   
    frame1Pen.setStyle(Qt::SolidLine);     
    frame1Brush.setColor(QColor(233, 137, 62));
    frame1Brush.setStyle(Qt::SolidPattern);

    frame2Pen.setWidth(2);
    frame2Pen.setColor(QColor(160, 82, 45));   
    frame2Pen.setStyle(Qt::SolidLine);     
    frame2Brush.setColor(QColor(222, 184, 135));
    frame2Brush.setStyle(Qt::SolidPattern);

    // set the bar
    barPen.setStyle(Qt::NoPen);
    barBrush.setColor(QColor(34,139,34));
    barBrush.setStyle(Qt::SolidPattern);

    // timer start
    timer.callOnTimeout(this, [=]{ MyProgressBar::update(); });
    timer.start(1);
}

void MyProgressBar::draw(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::TextAntialiasing, false);

    // draw the frame
    painter->setPen(frame1Pen);
    painter->setBrush(frame1Brush);
    painter->drawRoundedRect(frame1Rect, 5, 5);
    painter->setPen(frame2Pen);
    painter->setBrush(frame2Brush);
    painter->drawRoundedRect(frame2Rect, 5, 5); 

    // draw the bar
    if(value==0) return;
    painter->setPen(barPen);
    painter->setBrush(barBrush);
    painter->drawRoundedRect(barRect, 3, 3);

}

void MyProgressBar::update()
{
    value-=1;
    double barLength = (width-2*frameWidth-4)*value/time;
    barRect.setRect(x+frameWidth+2, y+frameWidth+2, barLength, height-2*frameWidth-4);
}
