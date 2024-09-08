#include "Game.h"
#include "Hero.h"
#include "Manager.h"
#include "Map.h"
#include "MyProgressBar.h"
#include "ScoreBoard.h"
#include <QRandomGenerator>
#include <QSet>
#include <QIcon>
#include <QTimer>
#include <QDebug>

// the Box Matrix is M*N
int M = 8, N = 6;

Hero *player1;
Map *gameMap;
MyProgressBar* progressBar;
ScoreBoard* scoreBoard1;

// 游戏实现
Game::Game(QWidget *parent)
    : QWidget(parent)
{
    QTimer *timer = new QTimer(this);
    timer->callOnTimeout(this, [=](){
        updateGame();
        update(); });
    timer->start(1000 / GAME_FPS);
}

void Game::run()
{
    int winWidth = MAP_WIDTH * CUBE_LENGTH;
    int winHeight = MAP_HRIGHT * CUBE_LENGTH;
    initGame(winWidth, winHeight, "QLink", QIcon(GAME_ICON));
    this->show();
}

void Game::initGame(int w, int h,
                    const QString &title, const QIcon &icon)
{
    // Set the Window's Size, Title and Icon
    setFixedSize(w, h);    // size
    setWindowTitle(title); // title
    if (!icon.isNull())    // icon
    {
        setWindowIcon(icon);
    }

    // generate map
    gameMap = new Map();

    // generate player
    player1 = new Hero(MAP_BLOCK_LEFT + 5, MAP_BLOCK_UP + 5);
    Mgr->addEntity(player1);

    // generate Box matric
    generateBox();

    // trigger timer start
    triggerElapsedTimer.start();

    // generate the Progress Bar
    progressBar = new MyProgressBar((CUBE_LENGTH*MAP_WIDTH-MY_PROGRESS_BAR_WIDTH)/2, CUBE_LENGTH, 60);
    connect(progressBar, &MyProgressBar::signalEnd, this, &Game::finishGame);

    // generate the scoreBoard
    scoreBoard1 = new ScoreBoard(0, 0);
    connect(this, &Game::signalScore, scoreBoard1, &ScoreBoard::addScore);
}

void Game::drawGame(QPainter *painter)
{
    gameMap->draw(painter);
    progressBar->draw(painter);
    scoreBoard1->draw(painter);
    Mgr->draw(painter);
}

void Game::finishGame()
{
    qInfo() << "GOOD GAME!";
}

void Game::updateGame()
{
    Mgr->update();
    collitionDetect();
}

void Game::cleanGame()
{
}

void Game::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    drawGame(&painter);
}

void Game::closeEvent(QCloseEvent *event)
{
    this->cleanGame();
}

void Game::keyPressEvent(QKeyEvent *event)
{
    if (player1->getState() == HS_stand_down ||
        player1->getState() == HS_stand_up ||
        player1->getState() == HS_stand_left ||
        player1->getState() == HS_stand_right)
    {
        switch (event->key())
        {
        case Qt::Key_W:
            player1->setState(HS_move_up);
            player1->velocity.setX(0);
            player1->velocity.setY(-1);
            break;
        case Qt::Key_A:
            player1->setState(HS_move_left);
            player1->velocity.setX(-1);
            player1->velocity.setY(0);
            break;
        case Qt::Key_S:
            player1->setState(HS_move_down);
            player1->velocity.setX(0);
            player1->velocity.setY(1);
            break;
        case Qt::Key_D:
            player1->setState(HS_move_right);
            player1->velocity.setX(1);
            player1->velocity.setY(0);
            break;
        }
    }
}

void Game::keyReleaseEvent(QKeyEvent *event)
{
    if (player1->getState() == HS_move_down ||
        player1->getState() == HS_move_up ||
        player1->getState() == HS_move_left ||
        player1->getState() == HS_move_right)
    {
        switch (event->key())
        {
        case Qt::Key_W:
            player1->setState(HS_stand_up);
            player1->velocity.setX(0);
            player1->velocity.setY(0);
            break;
        case Qt::Key_A:
            player1->setState(HS_stand_left);
            player1->velocity.setX(0);
            player1->velocity.setY(0);
            break;
        case Qt::Key_S:
            player1->setState(HS_stand_down);
            player1->velocity.setX(0);
            player1->velocity.setY(0);
            break;
        case Qt::Key_D:
            player1->setState(HS_stand_right);
            player1->velocity.setX(0);
            player1->velocity.setY(0);
            break;
        }
    }
}

// 功能函数
void Game::generateBox()
{
    // generate Box matrix
    for (int i = 1; i <= M; ++i)
        for (int j = 1; j <= N; ++j)
            boxMatrix[i][j] = QRandomGenerator::global()->bounded(BOX_COLOR_NUM) + 1;

    // generate Box
    int startX = MAP_BLOCK_LEFT + (MAP_BLOCK_RIGHT - MAP_BLOCK_LEFT - N * CUBE_LENGTH) / 2;
    int startY = MAP_BLOCK_UP + (MAP_BLOCK_DOWN - MAP_BLOCK_UP - M * CUBE_LENGTH) / 2;
    int x, y;

    for (int i = 0; i < M; ++i)
    {
        y = startY + i * CUBE_LENGTH;
        for (int j = 0; j < N; ++j)
        {
            x = startX + j * CUBE_LENGTH;
            Box *box = new Box(x, y, (BoxColor)boxMatrix[i + 1][j + 1]);
            box->setMatrixPosition(i + 1, j + 1);
            Mgr->addEntity(box);
        }
    }
}

void Game::collitionDetect()
{
    QSet<Box *> triggeredBoxes;
    // dect all the collision
    for (auto i : Mgr->getEntity(ET_box))
    {
        Box *foo = (Box *)i;
        if (player1->intersects(foo->getCollider()))
        {
            player1->collideEvent();
            triggeredBoxes.insert(foo);
        }
    }

    // if only one collision && trigger elapse>500 , then triggered
    if (triggeredBoxes.size() != 1)
        return;
    if (triggerElapsedTimer.elapsed() < 500)
        return;
    triggerElapsedTimer.restart();

    // boxes' trigger event
    Box *foo = *triggeredBoxes.begin();
    foo->trigger();

    // player's trigger event
    if (player1->getTriggeredBox() == nullptr)
    {
        player1->addTriggeredBox(foo);
        qInfo() << "player1 get:" << player1->getTriggeredBox();
    }
    else
    {
        qInfo() << "player1 abandon:" << player1->getTriggeredBox();
        ElimateBox(player1->getTriggeredBox(), foo);
        player1->resetTriggeredBox();
    }
}

void Game::ElimateBox(Box *playerBox, Box *box)
{
    // the same box
    if (playerBox == box)
    {
        playerBox->cancelTrigger();
        return;
    }
    // not same color
    if (playerBox->getColor() != box->getColor())
    {
        connect(box, &Box::signalCancelTrigger, playerBox, &Box::cancelTrigger);
        connect(box, &Box::signalElimate, box, &Box::cancelTrigger);
        return;
    }
    // the same color
    if (playerBox->getColor() == box->getColor())
    {
    
        if (horizonElimatable(playerBox, box) || verticalElimatable(playerBox, box) || oneCornerElimatable(playerBox, box) || twoCornerElimatable(playerBox, box))
        {
            qInfo() << "elimate Box:" << playerBox;
            qInfo() << "elimate Box:" << box;
            connect(box, &Box::signalElimate, playerBox, &Box::elimate);
            connect(box, &Box::signalElimate, box, &Box::elimate);
            boxMatrix[playerBox->getR()][playerBox->getC()] = 0;
            boxMatrix[box->getR()][box->getC()] = 0;

            // score
            score(2);
        }
        else
        {
            connect(box, &Box::signalCancelTrigger, playerBox, &Box::cancelTrigger);
            connect(box, &Box::signalElimate, box, &Box::cancelTrigger);
        }
    }
}

// 判定函数
bool Game::horizonElimatable(int r1, int c1, int r2, int c2)
{
    if (r1 != r2)
        return false;

    // ensure c1<c2
    if (c1 > c2)
    {
        int foo = c1;
        c1 = c2;
        c2 = foo;
    }

    for (int i = c1 + 1; i < c2; ++i)
    {
        if (boxMatrix[r1][i] != 0)
            return false;
    }

    return true;
    return false;
}

bool Game::horizonElimatable(const Box *box1, const Box *box2)
{
    int r1 = box1->getR();
    int c1 = box1->getC();
    int r2 = box2->getR();
    int c2 = box2->getC();

    return horizonElimatable(r1, c1, r2, c2);
}

bool Game::verticalElimatable(int r1, int c1, int r2, int c2)
{
    if (c1 != c2)
        return false;

    // ensure c1<c2
    if (r1 > r2)
    {
        int foo = r1;
        r1 = r2;
        r2 = foo;
    }

    for (int i = r1 + 1; i < r2; ++i)
    {
        if (boxMatrix[i][c1] != 0)
            return false;
    }

    return true;
}

bool Game::verticalElimatable(const Box *box1, const Box *box2)
{
    int r1 = box1->getR();
    int c1 = box1->getC();
    int r2 = box2->getR();
    int c2 = box2->getC();

    return verticalElimatable(r1, c1, r2, c2);
}

bool Game::oneCornerElimatable(const Box *box1, const Box *box2)
{
    int r1 = box1->getR();
    int c1 = box1->getC();
    int r2 = box2->getR();
    int c2 = box2->getC();

    // find the corner in the same r with box1
    // 2     2
    // |--1--|
    // 2     2
    for (int i = c1 + 1; i <= N && boxMatrix[r1][i] == 0; ++i)
        if (verticalElimatable(r1, i, r2, c2))
            return true;
    for (int i = c1 - 1; i >= 1 && boxMatrix[r1][i] == 0; --i)
        if (verticalElimatable(r1, i, r2, c2))
            return true;
    // find the corner in the same c with box1
    //  2---2
    //    1
    //  2---2
    for (int i = r1 + 1; i <= M && boxMatrix[i][c1] == 0; ++i)
        if (horizonElimatable(i, c1, r2, c2))
            return true;
    for (int i = r1 - 1; i >= 1 && boxMatrix[i][c1] == 0; --i)
        if (horizonElimatable(i, c1, r2, c2))
            return true;

    return false;
}

bool Game::twoCornerElimatable(const Box *box1, const Box *box2)
{
    int r1 = box1->getR();
    int c1 = box1->getC();
    int r2 = box2->getR();
    int c2 = box2->getC();

    // find the corner in same r with box1
    //  2-.-2
    //    |
    // 1--.
    //    |
    //  2-.-2
    for (int i = c1 + 1; i <= N + 1 && boxMatrix[r1][i] == 0; ++i)
    {
        // corner1(r1, i), corner2(r2, i)
        if (verticalElimatable(r1, i, r2, i) && horizonElimatable(r2, i, r2, c2))
            return true;
    }
    //  2-.-2
    //    |
    //    .--1
    //    |
    //  2-.-2
    for (int i = c1 - 1; i >= 0 && boxMatrix[r1][i] == 0; --i)
    {
        // corner1(r1, i), corner2(r2, i)
        if (verticalElimatable(r1, i, r2, i) && horizonElimatable(r2, i, r2, c2))
            return true;
    }

    // find the corner in the same c with box1
    // 2     2
    // .--.--.
    // 2  |  2
    //    1
    for (int i = r1 - 1; i >= 0 && boxMatrix[i][c1] == 0; --i)
    {
        // corner1(i, c1) corner2(i, c2)
        if (horizonElimatable(i, c1, i, c2) && verticalElimatable(i, c2, r2, c2))
            return true;
    }
    //    1
    // 2  |  2
    // .--.--.
    // 2     2
    for (int i = r1 + 1; i <= M + 1 && boxMatrix[i][c1] == 0; ++i)
    {
        // corner1(i, c1) corner2(i, c2)
        if (horizonElimatable(i, c1, i, c2) && verticalElimatable(i, c2, r2, c2))
            return true;
    }

    return false;
}

// slots
void Game::score(int x)
{
    emit signalScore(x);
}
