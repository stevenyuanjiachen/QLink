#pragma once 

#include <QWidget>
#include <QIcon>
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include "Box.h"
#include <QElapsedTimer>
#include <QTimer>
#include <QLine>

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
    void generateItem();
    void boxCollitionDect();
    void itemCollitionDect();
    void ElimateBox(Box *playerBox, Box *box);
    void score(int x);
    void solubleCheck();
    void addLine(int r1, int c1, int r2, int c2);
    void drawPath(QPainter* painter);
    void drawPauseMenu(QPainter* painter);

    // 判定函数
    bool elimatable(const Box* box1, const Box* box2, bool showPath = false);
    bool horizonElimatable(int r1, int c1, int r2, int c2, bool showPath = false);
    bool horizonElimatable(const Box* box1, const Box* box2, bool showPath = false);
    bool verticalElimatable(int r1, int c1, int r2, int c2, bool showPath = false);
    bool verticalElimatable(const Box* box1, const Box* box2, bool showPath = false);
    bool oneCornerElimatable(const Box* box1, const Box* box2, bool showPath = false);
    bool twoCornerElimatable(const Box* box1, const Box* box2, bool showPath = false);
    
signals:
    void signalPause();
    void signalContinue();
    void signalScore(int x);
    void signalAddLine(int r1, int c1, int r2, int c2);

protected:
    // 事件处理
    void paintEvent(QPaintEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    GameState state;
    int boxMatrix[MAX_M+2][MAX_N+2] = {};
    QElapsedTimer triggerElapsedTimer;
    QElapsedTimer showPathElapsedTimer;
    QTimer itemGenerateTimer; 
    QSet<QLine*> lineSet;  

};
