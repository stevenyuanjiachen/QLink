#include "Game.h"
#include "Hero.h"
#include "Manager.h"
#include "Map.h"
#include <QRandomGenerator>
#include <QSet>
#include <QTimer>
#include <QDebug>

const int BOX_PLACE = 5;
// the Box Matrix is M*N
int M=8, N=6;


Hero *player1;
Map* gameMap;

// 游戏实现
Game::Game(QWidget *parent)
    : QWidget(parent)
{
    QTimer *timer = new QTimer(this);
    timer->callOnTimeout(this, [=](){
        updateGame();
        update(); });
    timer->start(1000 / 60); // 帧率60
}

void Game::run()
{
    initGame(BACKGROUND_HRIGHT*CUBE_LENGTH, BACKGROUND_WIDTH*CUBE_LENGTH, "QLink");
    this->show();
}

void Game::initGame( int w, int h, 
    const QString &title, const QIcon &icon)
{
    // Set the Window's Size, Title and Icon
    setFixedSize(w, h); // size
    setWindowTitle(title); // title
    if (!icon.isNull()) // icon
    {
        setWindowIcon(icon);
    }

    // generate map
    gameMap = new Map();
    Mgr->addEntity(gameMap);
    
    // generate player
    player1 = new Hero(0, 0);
    Mgr->addEntity(player1);

    // generate Box matric
    generateBox();

    // trigger timer start
    triggerElapsedTimer.start();
}

void Game::drawGame(QPainter *painter)
{
    Mgr->draw(painter);
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
    int startX = (BACKGROUND_WIDTH*CUBE_LENGTH - N*CUBE_LENGTH)/2;
    int startY = (BACKGROUND_HRIGHT*CUBE_LENGTH - M*CUBE_LENGTH)/2;
    int x, y;

    for(int i=0; i<M; ++i)
    {
        y = startY + i*CUBE_LENGTH;
        for(int j=0; j<N; ++j)
        {
            x = startX + j*CUBE_LENGTH;
            int color = QRandomGenerator::global()->bounded(BOX_COLOR_NUM);
            Mgr->addEntity(new Box(x, y, (BoxColor)color));
        }
    }
}

void Game::collitionDetect()
{
    QSet<Box*> triggeredBoxes;
    // dect all the collision
    for(auto i: Mgr->getEntity(ET_box))
    {
        Box* foo = (Box*)i;
        if(player1->intersects(foo->getCollider()))
        {
            player1->collideEvent();
            triggeredBoxes.insert(foo);
        }
    }
    
    // if only one collision && trigger elapse>500 , then triggered
    if(triggeredBoxes.size()!=1) return;
    if(triggerElapsedTimer.elapsed()<500) return;
    triggerElapsedTimer.restart();
    
    // boxes' trigger event
    Box* foo = *triggeredBoxes.begin();
    foo->trigger();

    // player's trigger event
    if(player1->getTriggeredBox()==nullptr)
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
    if(playerBox==box) 
    {
        playerBox->cancelTrigger();
        return;
    }
    else if(playerBox->getColor() == box->getColor())
    {
        qInfo() << "elimate Box:" << playerBox;
        qInfo() << "elimate Box:" << box;
        
        connect(box, &Box::signalElimate, playerBox, &Box::elimate);
        connect(box, &Box::signalElimate, box, &Box::elimate);
    }
    else if(playerBox->getColor() != box->getColor())
    {
        connect(box, &Box::signalCancelTrigger, playerBox, &Box::cancelTrigger);
        connect(box, &Box::signalElimate, box, &Box::cancelTrigger);
    }
}
