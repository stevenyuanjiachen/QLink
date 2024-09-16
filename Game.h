#pragma once

#include <QElapsedTimer>
#include <QIcon>
#include <QKeyEvent>
#include <QLine>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>

#include "MyEntity"
#include "MyWidget"

const int GAME_FPS = 60;
const int MAX_M = 10;
const int MAX_N = 10;

enum GameState { GS_start, GS_single_mode, GS_double_mode, GS_pause, GS_finish };

class Game : public QWidget {
    Q_OBJECT
public:
    explicit Game(QWidget *parent = nullptr);
    void run();
    // Game Progress
    void initGame(const int w, const int h, const QString &title, const QIcon & = QIcon());
    void newGame(int gamemode, int m, int n, int time);
    void updateGame();
    void drawGame(QPainter *painter);
    void finishGame();
    void cleanGame();
    void pauseGame();
    void continueGame();
    void saveGame();
    void loadGame();
    void toStartMenu();

public:
    // Tools
    void generateBox(bool generateMatrix = true);
    void generateItem();
    void boxCollitionDect1();
    void boxCollitionDect2();
    void itemCollitionDect();
    void ElimateBox(Box *playerBox, Box *box, int playerID = 1);
    void triggerBox(int r, int c);
    void shuffleBox();
    void adjustBox(Box *box);
    void solubleCheck();
    void score(int x, int playerID);
    void addLine(int r1, int c1, int r2, int c2, int playerID = 0);
    void drawPath1(QPainter *painter);
    void drawPath2(QPainter *painter);
    void takeScreenShoot();
    void drawShadow(QPainter *painter);
    void saveGameState(const QString &filePath);
    void loadGameState(const QString &filePath);
    void saveBoxes(const QString &filePath);
    void loadBoxes(const QString &filePath);
    void saveItems(const QString &filePath);
    void loadItems(const QString &filePath);
    void setBoxMatrix(int matrix[MAX_M + 2][MAX_N + 2], int m, int n);
    // Decision Functions
    bool elimatable(int r1, int c1, int r2, int c2, int showPath = 0);
    bool elimatable(const Box *box1, const Box *box2, int showPath = 0);
    bool horizonElimatable(int r1, int c1, int r2, int c2, int showPath = 0);
    bool horizonElimatable(const Box *box1, const Box *box2, int showPath = 0);
    bool verticalElimatable(int r1, int c1, int r2, int c2, int showPath = 0);
    bool verticalElimatable(const Box *box1, const Box *box2, int showPath = 0);
    bool oneCornerElimatable(int r1, int c1, int r2, int c2, int showPath = 0);
    bool oneCornerElimatable(const Box *box1, const Box *box2, int showPath = 0);
    bool twoCornerElimatable(int r1, int c1, int r2, int c2, int showPath = 0);
    bool twoCornerElimatable(const Box *box1, const Box *box2, int showPath = 0);
    // Judge whether the character can move to the box
    bool canReachEdge(const int r, const int c);

signals:
    void signalPause();
    void signalContinue();
    void signalFinish(int gamemode, int score);
    void signalScore1(int x);
    void signalScore2(int x);
    void signalKeyRelease(QKeyEvent *event);

protected:
    // QWidget's slots
    void paintEvent(QPaintEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    GameState state;
    GameState gameMode;
    int M = 8, N = 8;
    int boxMatrix[MAX_M + 2][MAX_N + 2] = {};
    QElapsedTimer triggerElapsedTimer1; // Preventing Repeat Triggers
    QElapsedTimer triggerElapsedTimer2;
    QElapsedTimer showPathElapsedTimer1; // Control the Path time
    QElapsedTimer showPathElapsedTimer2;
    QElapsedTimer itemGenerateElapsedTimer;
    QSet<QLine *> lineSet1;
    QSet<QLine *> lineSet2;
    QPixmap screenshot;

    Hero *player1, *player2;
    Map *gameMap;
    MyProgressBar *progressBar;
    ScoreBoard *scoreBoard1, *scoreBoard2;
    PauseMenu *pauseMenu;
    StartMenu *startMenu;
    FinishMenu *finishMenu;
};
