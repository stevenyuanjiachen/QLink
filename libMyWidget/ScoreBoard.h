#pragma once

#include <QObject>
#include <QPainter>
#include <QPixmap>
#include <QPen>
#include <QTimer>
#include <QLinearGradient>
#include <QFontDatabase>
#include "MyWidgetResList.h"

class ScoreBoard: public QObject
{
    Q_OBJECT;
public:
    ScoreBoard(int x, int y, bool flip = false);
    ~ScoreBoard();
    void draw(QPainter* painter);
    void update();
    void addScore(int x);

private: 
    int x, y, score;
    bool flip;
    // the frame
    QPixmap scoreBoardFrame;
    // the text
    QString text;
    int textX, textY;
    QFont textFont;
    QBrush textBrush;
    QLinearGradient yellowGradient; //text color
    //timer
    QTimer timer;
};