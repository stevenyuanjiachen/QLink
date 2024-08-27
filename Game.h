#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QIcon>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include "Sprite.h"
#include "Box.h"

class QPainter;

class Game : public QWidget
{
    Q_OBJECT
public:
    // 游戏实现
    explicit Game(QWidget* parent = nullptr);
    void run();
    void initGame(int w, int h, const QString& title,const QIcon& = QIcon());
    void updateGame();
    void drawGame(QPainter* painter);
    void cleanGame();
public:
    // 功能函数
    void generateBox();
    void collitionDetect();
    void ElimateBox(Box* box1, Box* box2);
protected:
    // 事件处理
    void paintEvent(QPaintEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
};

#endif