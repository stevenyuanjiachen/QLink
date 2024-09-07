#include "MyProgressBar.h"
#include "MyWidgetResList.h"

MyProgressBar::MyProgressBar(int x, int y, double time)
    : QObject(nullptr), x(x), y(y),
      time(time * 1000), value(time * 1000)
{
    // set the frame
    frame = PROGRESSBAR_IMAGE;

    // set the bar
    barX = x + FRAME_INTERVAL_LEFT;
    barY = y + FRAME_INTERVAL_UP;
    barWidth = frame.width() - FRAME_INTERVAL_LEFT -12;
    barHeight = frame.height() - 2 * FRAME_INTERVAL_UP;
    barRect.setRect(barX, barY, barWidth, barHeight);
    barPen.setStyle(Qt::NoPen);
    barBrush.setColor(QColor(237, 216, 91));
    barBrush.setStyle(Qt::SolidPattern);

    // set the text
    textX = x + frame.width()/2-6;
    textY = y + frame.height()/2 + 7;
    textPen.setColor(QColor(81, 47, 54));
    int fontId = QFontDatabase::addApplicationFont(PIXEL_FONT);
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    textFont.setFamily(fontFamily);
    textFont.setPixelSize(12);

    // timer start
    timer.callOnTimeout(this, [=]
                        { MyProgressBar::update(); });
    timer.start(1);
}

void MyProgressBar::draw(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::TextAntialiasing, false);

    // draw the frame
    painter->drawPixmap(x, y, frame);

    // draw the bar
    if (value > 0)
    {
        painter->setPen(barPen);
        painter->setBrush(barBrush);
        painter->drawRoundedRect(barRect, 3, 3);
    }

    // draw the text
    painter->setPen(textPen);
    painter->setFont(textFont);
    painter->drawText(textX, textY, timeText);
}

void MyProgressBar::update()
{
    value -= 1;

    // update the bar
    double barLength = barWidth * value / time;
    barRect.setRect(barX, barY, barLength, barHeight);

    // update the text
    int totalSeconds = value / 1000;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    // Format time as mm:ss
    timeText = QString("%1:%2")
                           .arg(minutes, 2, 10, QChar('0'))  // Ensure two digits for minutes
                           .arg(seconds, 2, 10, QChar('0')); // Ensure two digits for seconds
    if(value<=0) timeText = "00:00";
}
