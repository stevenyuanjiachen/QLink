#include "ScoreBoard.h"
#include <QPainterPath>
#include <QFontMetrics>

ScoreBoard::ScoreBoard(int x, int y, bool flip) : x(x), y(y), flip(flip), score(0)
{
    // set the frame
    scoreBoardFrame.load(SCOREBOARD_IMAGE);

    // set the text
    text = QString("%1").arg(score, 3, 10, QChar('0'));

    // set the text color
    yellowGradient.setColorAt(0.0, QColor(255, 223, 136)); // 顶部较亮的黄色
    yellowGradient.setColorAt(1.0, QColor(255, 182, 40));  // 底部较深的黄色

    // set the text font
    int fontId = QFontDatabase::addApplicationFont(PIXEL_FONT);
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    textFont.setFamily(fontFamily);
    textFont.setPixelSize(12);

    // set the text location
    QFontMetrics fm(textFont);
    int textWidth = fm.horizontalAdvance(text);
    int textHeight = fm.height();
    textX = x + SCOREBOARD_INTERVAL_LEFT + (SCOREBOARD_INTERVAL_RIGHT - SCOREBOARD_INTERVAL_LEFT - textWidth) / 2;
    textY = y + SCOREBOARD_INTERVAL_UP + (SCOREBOARD_INTERVAL_DOWN - SCOREBOARD_INTERVAL_UP + textHeight) / 2 + 3;

    // auto update
    timer.callOnTimeout(this, &ScoreBoard::update);
    timer.start(1000 / 60);
}

ScoreBoard::~ScoreBoard()
{
}

void ScoreBoard::draw(QPainter *painter)
{
    // draw the frame
    painter->drawPixmap(x, y, scoreBoardFrame);

    // draw the text by the painter path
    QPainterPath textPath;
    textPath.addText(textX, textY, textFont, text);
    painter->setPen(Qt::NoPen);
    painter->setBrush(yellowGradient);
    painter->drawPath(textPath);
}

void ScoreBoard::update()
{
    text = QString("%1").arg(score, 3, 10, QChar('0'));
}

void ScoreBoard::addScore(int x)
{
    score += x;
}
