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
    ScoreBoard(int x, int y, bool flip = false, int id = 1);
    ~ScoreBoard();
    void draw(QPainter* painter);
    void update();
    void init();
    void addScore(int x);
    int getScore() const { return score; }
    void saveState(const QString& filePath);
    void loadState(const QString& filePath);

private: 
    int scoreBoardID;
    int x, y, score;
    bool flip;
    QPixmap scoreBoardFrame; // the frame
    QString text; // the text
    int textX, textY;
    QFont textFont;
    QBrush textBrush;
    QLinearGradient yellowGradient; // text color
    QTimer timer;
};