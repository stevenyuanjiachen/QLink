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
    GS_single_mode,
    GS_double_mode,
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
    void pauseGame();
    void continueGame();
    void saveGame();
    void loadGame();

public:
    // 功能函数
    void generateBox(bool generateMatrix = true);
    void generateItem();
    void boxCollitionDect1();
    void boxCollitionDect2();
    void itemCollitionDect();
    void ElimateBox(Box *playerBox, Box *box, int playerID = 1);
    void score(int x, int playerID);
    void triggerBox(int r, int c);
    void shuffleBox();
    void solubleCheck();
    void addLine(int r1, int c1, int r2, int c2, int playerID=0);
    void drawPath1(QPainter* painter);
    void drawPath2(QPainter* painter);
    void drawPauseMenu(QPainter* painter);
    void saveBoxes(const QString& filePath);
    void loadBoxes(const QString& filePath);
    void saveItems(const QString& filePath);
    void loadItems(const QString& filePath);

    // 判定函数
    bool elimatable(const Box* box1, const Box* box2, int showPath = 0);
    bool horizonElimatable(int r1, int c1, int r2, int c2, int showPath = 0);
    bool horizonElimatable(const Box* box1, const Box* box2, int showPath = 0);
    bool verticalElimatable(int r1, int c1, int r2, int c2, int showPath = 0);
    bool verticalElimatable(const Box* box1, const Box* box2, int showPath = 0);
    bool oneCornerElimatable(const Box* box1, const Box* box2, int showPath = 0);
    bool twoCornerElimatable(const Box* box1, const Box* box2, int showPath = 0);
    bool canReachEdge(const int r, const int c);
    
signals:
    void signalPause();
    void signalContinue();
    void signalScore1(int x);
    void signalScore2(int x);

protected:
    // 事件处理
    void paintEvent(QPaintEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    GameState state;
    GameState gameMode;
    int boxMatrix[MAX_M+2][MAX_N+2] = {};
    QElapsedTimer triggerElapsedTimer1;
    QElapsedTimer triggerElapsedTimer2;
    QElapsedTimer showPathElapsedTimer1;
    QElapsedTimer showPathElapsedTimer2;
    QTimer itemGenerateTimer; 
    QSet<QLine*> lineSet1;
    QSet<QLine*> lineSet2;
    QPixmap screenshot;
};
