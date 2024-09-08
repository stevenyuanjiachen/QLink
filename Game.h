#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QIcon>
#include <QPainter>
#include <QKeyEvent>
#include "Box.h"
#include <QElapsedTimer>

const int GAME_FPS = 60;
const int MAX_M = 10;
const int MAX_N = 10;

enum GameState{
    GS_start,
    GS_running,
    GS_pause,
    GS_finish  
};

class Game : public QWidget
{
    Q_OBJECT
public:
    // 游戏实现
    explicit Game(QWidget *parent = nullptr);
    void run();
    void initGame(const int w,const int h, const QString &title, const QIcon & = QIcon());
    void updateGame();
    void drawGame(QPainter *painter);
    void finishGame();
    void cleanGame();

public:
    // 功能函数
    void generateBox();
    void collitionDetect();
    void ElimateBox(Box *playerBox, Box *box);
    void score(int x);
    void solubleCheck();

    // 判定函数
    bool elimatable(const Box* box1, const Box* box2);
    bool horizonElimatable(int r1, int c1, int r2, int c2);
    bool horizonElimatable(const Box* box1, const Box* box2);
    bool verticalElimatable(int r1, int c1, int r2, int c2);
    bool verticalElimatable(const Box* box1, const Box* box2);
    bool oneCornerElimatable(const Box* box1, const Box* box2);
    bool twoCornerElimatable(const Box* box1, const Box* box2);
    
signals:
    void signalScore(int x);

protected:
    // 事件处理
    void paintEvent(QPaintEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QElapsedTimer triggerElapsedTimer;
    int boxMatrix[MAX_M+2][MAX_N+2] = {};
    GameState state;
};

#endif