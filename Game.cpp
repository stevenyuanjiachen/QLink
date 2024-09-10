#include "Game.h"
#include "Hero.h"
#include "Manager.h"
#include "Shuffle.h"
#include "Flash.h"
#include "Map.h"
#include "MyProgressBar.h"
#include "ScoreBoard.h"
#include <PauseMenu.h>
#include <QRandomGenerator>
#include <QSet>
#include <QIcon>
#include <QTimer>
#include <QDebug>
#include <QPoint>

// the Box Matrix is M*N
int M = 8, N = 8;

Hero *player1;
Map *gameMap;
MyProgressBar *progressBar;
ScoreBoard *scoreBoard1;
PauseMenu* pauseMenu;

// 游戏实现
Game::Game(QWidget *parent)
    : QWidget(parent), state(GS_start)
{
    // game timer
    QTimer *timer = new QTimer(this);
    timer->callOnTimeout(this, [=]()
                         {
        updateGame();
        update(); });
    timer->start(1000 / GAME_FPS);

    itemGenerateTimer.callOnTimeout(this, &Game::generateItem);
    itemGenerateTimer.start(5000);
}

void Game::run()
{
    int winWidth = MAP_WIDTH * CUBE_LENGTH;
    int winHeight = MAP_HEIGHT * CUBE_LENGTH;
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
    progressBar = new MyProgressBar((CUBE_LENGTH * MAP_WIDTH - MY_PROGRESS_BAR_WIDTH) / 2, CUBE_LENGTH, 60);
    connect(progressBar, &MyProgressBar::signalEnd, this, &Game::finishGame);
    connect(this, &Game::signalPause, progressBar, &MyProgressBar::pauseBar);
    connect(this, &Game::signalContinue, progressBar, &MyProgressBar::contineBar);

    // generate the scoreBoard
    scoreBoard1 = new ScoreBoard(0, 0);
    connect(this, &Game::signalScore, scoreBoard1, &ScoreBoard::addScore);

    // generate the puase menu
    pauseMenu = new PauseMenu(this, (MAP_WIDTH*CUBE_LENGTH-PAUSE_MENU_WIDTH)/2, (MAP_HEIGHT*CUBE_LENGTH-PAUSE_MENU_HEIGHT)/2);
    connect(pauseMenu, &PauseMenu::signalContinue, this, &Game::continueGame);
}

void Game::drawGame(QPainter *painter)
{
    switch (state)
    {
    case GS_running:
        gameMap->draw(painter);
        progressBar->draw(painter);
        scoreBoard1->draw(painter);
        Mgr->draw(painter);
        drawPath(painter);
        break;
    case GS_pause:
        gameMap->draw(painter);
        progressBar->draw(painter);
        scoreBoard1->draw(painter);
        Mgr->draw(painter);
        drawPath(painter);
        // pauseMenu
        drawPauseMenu(painter);
        pauseMenu->draw(painter);
        break;
    }
    
}

void Game::finishGame()
{
    state = GS_finish;
}

void Game::updateGame()
{
    switch (state)
    {
    case GS_start:
        state = GS_running;
        break;
    case GS_running:
        Mgr->update();
        boxCollitionDect();
        itemCollitionDect();
        solubleCheck();
        break;
    case GS_pause:
        emit signalPause();
        itemGenerateTimer.start(5000);
        break;
    case GS_finish:
        qInfo() << "Game Finish";
        break;
    }
}

void Game::cleanGame()
{
}

void Game::continueGame()
{
    emit signalContinue();
    state = GS_running;
    pauseMenu->hide();
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
    if(state == GS_pause) return;

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
    // player
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

    // pause
    if(event->key() == Qt::Key_Escape)
    {
        state = GS_pause;
    }

}

void Game::mouseReleaseEvent(QMouseEvent *event)
{
    if (!player1->haveFlash())
        return;

    QPoint mousePos = event->pos();
    QRect newCollider(mousePos.x() - 13, mousePos.y(), 26, 14);

    if (mousePos.x() - 13 < MAP_BLOCK_LEFT || mousePos.x() + 13 > MAP_BLOCK_RIGHT || mousePos.y() - 30 < MAP_BLOCK_UP || mousePos.y() + 14 > MAP_BLOCK_DOWN)
        return;

    for (auto i : Mgr->getEntity(ET_box))
    {
        Box *box = (Box *)i;
        if (newCollider.intersects(box->getCollider()))
            return;
    }

    player1->setPosition(mousePos.x() - 13, mousePos.y() - 30);
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

void Game::generateItem()
{
    Item *item;

    // random the type
    int itemType = QRandomGenerator::global()->bounded(2) + 1;
    switch (itemType)
    {
    case 1:
        item = new Shuffle();
        break;
    case 2:
        item = new Flash();
        break;
    }

    // random the location
    int x, y;
    QRect outRect(MAP_BLOCK_LEFT, MAP_BLOCK_UP,                     // postion
                  MAP_BLOCK_RIGHT - MAP_BLOCK_LEFT - item->width(), // width
                  MAP_BLOCK_DOWN - MAP_BLOCK_UP - item->height());  // height
    QRect inRect(MAP_BLOCK_LEFT + (MAP_BLOCK_RIGHT - MAP_BLOCK_LEFT - N * CUBE_LENGTH) / 2 - item->width(),
                 MAP_BLOCK_UP + (MAP_BLOCK_DOWN - MAP_BLOCK_UP - M * CUBE_LENGTH) / 2 - item->height(),
                 item->width() + N * CUBE_LENGTH,
                 item->height() + M * CUBE_LENGTH);
    while (true)
    {
        x = outRect.x() + QRandomGenerator::global()->bounded(outRect.width());
        y = outRect.y() + QRandomGenerator::global()->bounded(outRect.height());
        if (!inRect.contains(x, y))
            break;
    }
    item->setPosition(x, y);

    Mgr->addEntity(item);

    // random the time
    itemGenerateTimer.setInterval(1000 * QRandomGenerator::global()->bounded(3, 8));
}

void Game::boxCollitionDect()
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

void Game::itemCollitionDect()
{
    // dect shuffle
    for (auto i : Mgr->getEntity(ET_shuffle))
    {
        Shuffle *shuffle = (Shuffle *)i;
        if (player1->intersects(shuffle->getCollider()))
        {
            progressBar->addTime(1);
            shuffle->pickUp();
        }
    }

    // dect flash
    for (auto i : Mgr->getEntity(ET_flash))
    {
        Flash *flash = (Flash *)i;
        if (player1->intersects(flash->getCollider()))
        {
            player1->addBuff(BT_flash);
            flash->pickUp();
        }
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

        if (elimatable(playerBox, box, true))
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

void Game::score(int x)
{
    emit signalScore(x);
}

void Game::solubleCheck()
{
    int r, c;
    Box *box;
    QSet<Box *> boxes;

    for (auto i : Mgr->getEntity(ET_box))
    {
        box = (Box *)i;
        r = box->getR();
        c = box->getC();

        // 边缘检测
        if (boxMatrix[r - 1][c] != 0 && boxMatrix[r + 1][c] != 0 && boxMatrix[r][c - 1] != 0 && boxMatrix[r][c + 1] != 0)
            continue;

        if (boxes.count() != 0)
        {
            for (auto j : boxes)
            {
                if (elimatable(j, box))
                    return;
            }
        }
        boxes.insert(box);
    }

    finishGame();
}

void Game::addLine(int r1, int c1, int r2, int c2)
{
    int startX = MAP_BLOCK_LEFT + (MAP_BLOCK_RIGHT - MAP_BLOCK_LEFT - N * CUBE_LENGTH) / 2;
    int startY = MAP_BLOCK_UP + (MAP_BLOCK_DOWN - MAP_BLOCK_UP - M * CUBE_LENGTH) / 2;
    int x1 = startX + (c1 - 0.5) * CUBE_LENGTH;
    int y1 = startY + (r1 - 0.5) * CUBE_LENGTH;
    int x2 = startX + (c2 - 0.5) * CUBE_LENGTH;
    int y2 = startY + (r2 - 0.5) * CUBE_LENGTH;

    lineSet.insert(new QLine(x1, y1, x2, y2));
    showPathElapsedTimer.restart();
}

void Game::drawPath(QPainter *painter)
{
    if (lineSet.isEmpty())
        return;

    QPen pen;
    pen.setColor(QColor(173, 216, 230, 256 * 0.8));
    pen.setWidth(8);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);

    for (auto i : lineSet)
        painter->drawLine(*i);

    if (showPathElapsedTimer.elapsed() > 500)
        for (auto i : lineSet)
        {
            lineSet.remove(i);
            delete i;
        }
}

void Game::drawPauseMenu(QPainter* painter)
{
    QRect rect(0, 0, MAP_WIDTH*CUBE_LENGTH, MAP_HEIGHT*CUBE_LENGTH);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0, 0, 0, 100));
    painter->drawRect(rect);
}

// 判定函数
bool Game::elimatable(const Box *box1, const Box *box2, bool showPath)
{
    return horizonElimatable(box1, box2, showPath) || verticalElimatable(box1, box2, showPath) || oneCornerElimatable(box1, box2, showPath) || twoCornerElimatable(box1, box2, showPath);
}

bool Game::horizonElimatable(int r1, int c1, int r2, int c2, bool showPath)
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

    // draw path
    if (showPath == true)
        addLine(r1, c1, r2, c2);

    return true;
}

bool Game::horizonElimatable(const Box *box1, const Box *box2, bool showPath)
{
    int r1 = box1->getR();
    int c1 = box1->getC();
    int r2 = box2->getR();
    int c2 = box2->getC();

    return horizonElimatable(r1, c1, r2, c2, showPath);
}

bool Game::verticalElimatable(int r1, int c1, int r2, int c2, bool showPath)
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

    // draw path
    if (showPath == true)
        addLine(r1, c1, r2, c2);

    return true;
}

bool Game::verticalElimatable(const Box *box1, const Box *box2, bool showPath)
{
    int r1 = box1->getR();
    int c1 = box1->getC();
    int r2 = box2->getR();
    int c2 = box2->getC();

    return verticalElimatable(r1, c1, r2, c2, showPath);
}

bool Game::oneCornerElimatable(const Box *box1, const Box *box2, bool showPath)
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
        {
            if (showPath == true)
            {
                addLine(r1, i, r2, c2);
                addLine(r1, c1, r1, i);
            }
            return true;
        }
    for (int i = c1 - 1; i >= 1 && boxMatrix[r1][i] == 0; --i)
        if (verticalElimatable(r1, i, r2, c2))
        {
            if (showPath == true)
            {
                addLine(r1, i, r2, c2);
                addLine(r1, c1, r1, i);
            }
            return true;
        }
    // find the corner in the same c with box1
    //  2---2
    //    1
    //  2---2
    for (int i = r1 + 1; i <= M && boxMatrix[i][c1] == 0; ++i)
        if (horizonElimatable(i, c1, r2, c2))
        {
            if (showPath == true)
            {
                addLine(i, c1, r2, c2);
                addLine(r1, c1, i, c1);
            }
            return true;
        }
    for (int i = r1 - 1; i >= 1 && boxMatrix[i][c1] == 0; --i)
        if (horizonElimatable(i, c1, r2, c2))
        {
            if (showPath == true)
            {
                addLine(i, c1, r2, c2);
                addLine(r1, c1, i, c1);
            }
            return true;
        }
    return false;
}

bool Game::twoCornerElimatable(const Box *box1, const Box *box2, bool showPath)
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
        {
            if (showPath == true)
            {
                addLine(r1, c1, r1, i);
                addLine(r1, i, r2, i);
                addLine(r2, i, r2, c2);
            }
            return true;
        }
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
        {
            if (showPath == true)
            {
                addLine(r1, c1, r1, i);
                addLine(r1, i, r2, i);
                addLine(r2, i, r2, c2);
            }
            return true;
        }
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
        {
            if (showPath == true)
            {
                addLine(r1, c1, i, c1);
                addLine(i, c1, i, c2);
                addLine(i, c2, r2, c2);
            }
            return true;
        }
    }
    //    1
    // 2  |  2
    // .--.--.
    // 2     2
    for (int i = r1 + 1; i <= M + 1 && boxMatrix[i][c1] == 0; ++i)
    {
        // corner1(i, c1) corner2(i, c2)
        if (horizonElimatable(i, c1, i, c2) && verticalElimatable(i, c2, r2, c2))
        {
            if (showPath == true)
            {
                addLine(r1, c1, i, c1);
                addLine(i, c1, i, c2);
                addLine(i, c2, r2, c2);
            }
            return true;
        }
    }

    return false;
}
