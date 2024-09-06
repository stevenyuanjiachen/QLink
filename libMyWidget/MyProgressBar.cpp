#include "MyProgressBar.h"

MyProgressBar::MyProgressBar(int x, int y, int w, int h, double time):
    QObject(nullptr), x(x), y(y), width(w), height(h), offset(2),
    time(time*1000), value(time*1000)
{
    // set the location
    frameRect.setRect(x, y, w, h);
    barRect.setRect(x+offset, y+offset, w-2*offset, h-2*offset);

    // set the frame
    framePen.setWidth(1);
    framePen.setColor(QColor(210, 210, 210));   
    framePen.setStyle(Qt::SolidLine);     // 线的类型
    framePen.setCapStyle(Qt::FlatCap);    // 线端点样式
    framePen.setJoinStyle(Qt::BevelJoin); // 线的连接点样式
    frameBrush.setColor(QColor(190, 190, 190));
    frameBrush.setStyle(Qt::SolidPattern);

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
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing);

    // draw the frame
    painter->setPen(framePen);
    painter->setBrush(frameBrush);
    painter->drawRoundedRect(frameRect, 12, 12);

    // draw the bar
    if(value==0) return;
    painter->setPen(barPen);
    painter->setBrush(barBrush);
    painter->drawRoundedRect(barRect, 10, 10);

}

void MyProgressBar::update()
{
    value-=1;
    double barLength = (width-2*offset)*value/time;
    barRect.setRect(x+offset, y+offset, barLength, height-2*offset);
}
