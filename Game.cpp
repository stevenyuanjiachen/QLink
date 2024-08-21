#include "Game.h"
#include "Hero.h"
#include "Manager.h"
#include "Map.h"
#include "Box.h"
#include <QRandomGenerator>

const int BOX_PLACE = 5;

Hero *player1;
Map* gameMap;

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
    initGame(MAP_HEIGHT*CUBE_LENGTH, MAP_WIDTH*CUBE_LENGTH, "QLink");
    this->show();
}

void Game::initGame(
    int w, int h,
    const QString &title,
    const QIcon &icon)
{
    setFixedSize(w, h);
    setWindowTitle(title);
    if (!icon.isNull())
    {
        setWindowIcon(icon);
    }

    gameMap = new Map();
    Mgr->addEntity(gameMap);
    
    player1 = new Hero(0, 0);
    Mgr->addEntity(player1);

    generateBox();
}

void Game::drawGame(QPainter *painter)
{
    Mgr->draw(painter);
}

void Game::updateGame()
{
    Mgr->update();
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

void Game::generateBox()
{
    for(int i=BOX_PLACE; i<MAP_HEIGHT-BOX_PLACE; ++i)
    {
        for(int j=BOX_PLACE; j<MAP_WIDTH-BOX_PLACE; ++j)
        {
            int color = QRandomGenerator::global()->bounded(BOX_COLOR_NUM);
            Mgr->addEntity(new Box(j*CUBE_LENGTH, i*CUBE_LENGTH, (BoxColor)color));
        }
    }
}