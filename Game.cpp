#include "Game.h"
#include "Hero.h"
#include "Manager.h"
#include "Item.h"
#include "Map.h"
#include "MyProgressBar.h"
#include "ScoreBoard.h"
#include "PauseMenu.h"
#include "StartMenu.h"
#include "FinishMenu.h"
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QSet>
#include <QIcon>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QPoint>
#include <QScreen>
#include <QApplication>
#include <iostream>
#include <queue>

// 游戏实现
Game::Game(QWidget *parent)
    : QWidget(parent), state(GS_start)
{
    // game timer
    QTimer *timer = new QTimer(this);
    timer->callOnTimeout(this, &Game::updateGame);
    timer->start(1000 / GAME_FPS);
    
    setFocusPolicy(Qt::StrongFocus);
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
    if (!icon.isNull()) setWindowIcon(icon); // icon

    // generate map
    gameMap = new Map();
    
    // generate player
    player1 = new Hero(MAP_BLOCK_LEFT + 10, MAP_BLOCK_UP + 10);
    player2 = new Hero(MAP_BLOCK_RIGHT - player1->pixmap.width() - 10,
                       MAP_BLOCK_DOWN - player1->pixmap.height() - 10, 2);
    Mgr->addEntity(player1);
    triggerElapsedTimer1.start();
    triggerElapsedTimer2.start();

    // generate the Progress Bar
    progressBar = new MyProgressBar((CUBE_LENGTH * MAP_WIDTH - MY_PROGRESS_BAR_WIDTH) / 2, CUBE_LENGTH, 60);
    connect(progressBar, &MyProgressBar::signalEnd, this, &Game::finishGame);
    connect(this, &Game::signalPause, progressBar, &MyProgressBar::pauseBar);
    connect(this, &Game::signalContinue, progressBar, &MyProgressBar::contineBar);

    // generate the scoreBoard
    scoreBoard1 = new ScoreBoard(0, 0);
    scoreBoard2 = new ScoreBoard(0, 50, false, 2);
    connect(this, &Game::signalScore1, scoreBoard1, &ScoreBoard::addScore);
    connect(this, &Game::signalScore2, scoreBoard2, &ScoreBoard::addScore);

    // generate the puase menu
    pauseMenu = new PauseMenu(this, (MAP_WIDTH * CUBE_LENGTH - PAUSE_MENU_WIDTH) / 2, (MAP_HEIGHT * CUBE_LENGTH - PAUSE_MENU_HEIGHT) / 2);
    connect(pauseMenu, &PauseMenu::signalContinue, this, &Game::continueGame);
    connect(pauseMenu, &PauseMenu::signalSaveGame, this, &Game::saveGame);
    connect(pauseMenu, &PauseMenu::signalLoadGame, this, &Game::loadGame);
    connect(pauseMenu, &PauseMenu::signalQuitGame, this, &Game::toStartMenu);

    // generate the start menu
    startMenu = new StartMenu(this);
    connect(startMenu, &StartMenu::signalNewGame, this, &Game::newGame);
    connect(startMenu, &StartMenu::signalLoadGame, this, &Game::loadGame);
    connect(startMenu, &StartMenu::signalExit, this, &Game::close);
    connect(this, &Game::signalKeyRelease, startMenu, &StartMenu::keyReleaseEvent);

    // generate the finish menu
    finishMenu = new FinishMenu(this, (MAP_WIDTH * CUBE_LENGTH - PAUSE_MENU_WIDTH) / 2, (MAP_HEIGHT * CUBE_LENGTH - PAUSE_MENU_HEIGHT) / 2);
    connect(finishMenu, &FinishMenu::signalQuitGame, this, &Game::toStartMenu);
    connect(this, &Game::signalFinish, finishMenu, &FinishMenu::setLabel);
}

void Game::newGame(int gamemode, int m, int n, int time)
{
    state  = gameMode = (GameState) gamemode;
    M = m; N = n;
    
    Mgr->clean();

    // generate player
    player1 = new Hero(MAP_BLOCK_LEFT + 10, MAP_BLOCK_UP + 10);
    player2 = new Hero(MAP_BLOCK_RIGHT - player1->pixmap.width() - 10,
                       MAP_BLOCK_DOWN - player1->pixmap.height() - 10, 2);
    Mgr->addEntity(player1);
    triggerElapsedTimer1.start();
    if(gamemode==2) {   
        Mgr->addEntity(player2);
        triggerElapsedTimer2.start();
    }
    
    // generate Box
    generateBox();

    // generate items
    itemGenerateElapsedTimer.start();

    // init the scoreBoard
    scoreBoard1->init();
    scoreBoard2->init();

    // init the progress bar
    progressBar->init(time);

    continueGame();
    startMenu->hide();
}

void Game::drawGame(QPainter *painter)
{
    switch (state)
    {
    case GS_start:
        startMenu->draw(painter);
        break;
    case GS_single_mode:
        gameMap->draw(painter);
        progressBar->draw(painter);
        scoreBoard1->draw(painter);
        Mgr->draw(painter);
        drawPath1(painter);
        break;
    case GS_double_mode:
        gameMap->draw(painter);
        progressBar->draw(painter);
        scoreBoard1->draw(painter);
        scoreBoard2->draw(painter);
        Mgr->draw(painter);
        drawPath1(painter);
        drawPath2(painter);
        break;
    case GS_pause:
        drawShadow(painter);
        pauseMenu->draw(painter);
        break;
    case GS_finish:
        drawShadow(painter);
        finishMenu->draw(painter);
    }
}

void Game::finishGame()
{
    takeScreenShoot();
    if(gameMode == GS_single_mode) emit signalFinish(1, scoreBoard1->getScore());
    else if(gameMode == GS_double_mode)
    {
        if(scoreBoard1->getScore() > scoreBoard2->getScore()) 
            emit signalFinish(2, 1);
        if(scoreBoard1->getScore() < scoreBoard2->getScore()) 
            emit signalFinish(2, 2);
        if(scoreBoard1->getScore() == scoreBoard2->getScore()) 
            emit signalFinish(2, 3);
    }
    state = GS_finish;
}

void Game::updateGame()
{
    switch (state)
    {
    case GS_start:
        emit signalPause();
        startMenu->update();
        break;
    case GS_single_mode:
        Mgr->update();
        boxCollitionDect1();
        generateItem();
        itemCollitionDect();
        solubleCheck();
        break;
    case GS_double_mode:
        Mgr->update();
        boxCollitionDect1();
        boxCollitionDect2();
        generateItem();
        itemCollitionDect();
        solubleCheck();
        break;
    case GS_pause:
        break;
    case GS_finish:
        emit signalPause();
        break;
    }

    update();
}

void Game::cleanGame()
{
    Mgr->clean();
    delete scoreBoard1;
    delete scoreBoard2;
    delete progressBar;
    delete gameMap;
    delete pauseMenu;
    delete finishMenu;
    delete startMenu;
}

void Game::pauseGame()
{
    takeScreenShoot();
    player1->pauseHero();
    if(gameMode==GS_double_mode) player2->pauseHero();
    state = GS_pause;
    emit signalPause();
}

void Game::continueGame()
{
    emit signalContinue();
    state = gameMode;
    itemGenerateElapsedTimer.restart();
    player1->continueHero();
    if(gameMode==GS_double_mode) player2->continueHero();
    pauseMenu->hide();
    startMenu->hide();
    finishMenu->hide();
}

void Game::saveGame()
{
    const QString filePath = "../saves/save1.txt";

    // if save exist, then remove it
    if (QFile::exists(filePath))
    {
        if (!QFile::remove(filePath))
        {
            qDebug() << "无法删除目标文件!";
            return;
        }
    }

    // copy the template
    if (QFile::copy("../res/saveTemplate.txt", filePath))
        qDebug() << "文件复制成功!";
    else
        qDebug() << "文件复制失败!";

    saveGameState(filePath);
    saveBoxes(filePath);              // save box
    player1->saveHeroState(filePath); // save hero
    if(gameMode==GS_double_mode) player2->saveHeroState(filePath);
    saveItems(filePath);              // save item
    progressBar->saveState(filePath); // save progressBar
    scoreBoard1->saveState(filePath); // save scoreBoard
}

void Game::loadGame()
{
    const QString &filePath = "../saves/save1.txt";

    Mgr->clean(); // clean Game

    loadGameState(filePath); // load game
    loadBoxes(filePath);     // load boxes

    // load player 1
    player1 = new Hero(0, 0, 1);
    player1->loadHeroState(filePath);
    Mgr->addEntity(player1); 

    // load player 2
    if (gameMode == GS_double_mode)
    {
        player2 = new Hero(0, 0, 2);
        player2->loadHeroState(filePath);
        Mgr->addEntity(player2);
    }

    loadItems(filePath);              // load items
    itemGenerateElapsedTimer.restart();
    progressBar->loadState(filePath); // load progressBar
    scoreBoard1->loadState(filePath); // load scoreBoard1
    if(gameMode == GS_double_mode)
        scoreBoard2->loadState(filePath);

    continueGame();
}

void Game::toStartMenu()
{
    Mgr->clean();
    startMenu->show();
    pauseMenu->hide();
    finishMenu->hide();
    state = GS_start;
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
    if (state == GS_pause) return;

    // player 1
    if(!player1->haveDizzy() && !player1->haveFreeze())
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
    // player 1 have dizzy
    if(player1->haveDizzy() && !player1->haveFreeze()){
        switch (event->key())
        {
        case Qt::Key_W:
            player1->setState(HS_move_down);
            player1->velocity.setX(0);
            player1->velocity.setY(1);
            break;
        case Qt::Key_A:
            player1->setState(HS_move_right);
            player1->velocity.setX(1);
            player1->velocity.setY(0);
            break;
        case Qt::Key_S:
            player1->setState(HS_move_up);
            player1->velocity.setX(0);
            player1->velocity.setY(-1);
            break;
        case Qt::Key_D:
            player1->setState(HS_move_left);
            player1->velocity.setX(-1);
            player1->velocity.setY(0);
            break;
        }

    }

    // player 2
    if (this->state != GS_double_mode) return;
    if(!player2->haveDizzy() && !player2->haveFreeze()) {
        switch (event->key())
        {
        case Qt::Key_Up:
            player2->setState(HS_move_up);
            player2->velocity.setX(0);
            player2->velocity.setY(-1);
            break;
        case Qt::Key_Left:
            player2->setState(HS_move_left);
            player2->velocity.setX(-1);
            player2->velocity.setY(0);
            break;
        case Qt::Key_Down:
            player2->setState(HS_move_down);
            player2->velocity.setX(0);
            player2->velocity.setY(1);
            break;
        case Qt::Key_Right:
            player2->setState(HS_move_right);
            player2->velocity.setX(1);
            player2->velocity.setY(0);
            break;
        }
    }
    // player2 have dizzy
    if(player2->haveDizzy() && !player2->haveFreeze())
    {
        switch (event->key())
        {
        case Qt::Key_Down:
            player2->setState(HS_move_up);
            player2->velocity.setX(0);
            player2->velocity.setY(-1);
            break;
        case Qt::Key_Right:
            player2->setState(HS_move_left);
            player2->velocity.setX(-1);
            player2->velocity.setY(0);
            break;
        case Qt::Key_Up:
            player2->setState(HS_move_down);
            player2->velocity.setX(0);
            player2->velocity.setY(1);
            break;
        case Qt::Key_Left:
            player2->setState(HS_move_right);
            player2->velocity.setX(1);
            player2->velocity.setY(0);
            break;
        }
    }

}

void Game::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();

    // start menu
    if(state == GS_start) emit signalKeyRelease(event);

    // pause
    if(state == GS_double_mode || state == GS_single_mode)
        if (event->key() == Qt::Key_Escape) pauseGame();

    // player 1
    if (key == Qt::Key_W || key==Qt::Key_A || key==Qt::Key_S || key==Qt::Key_D) 
    {
        switch (player1->getState())
        {
        case HS_move_up:
            player1->setState(HS_stand_up);
            break;
        case HS_move_left:
            player1->setState(HS_stand_left);
            break;
        case HS_move_down:
            player1->setState(HS_stand_down);
            break;
        case HS_move_right:
            player1->setState(HS_stand_right);
            break;
        }
        player1->velocity.setX(0);
        player1->velocity.setY(0);
    }

    // player2
    if (this->state != GS_double_mode) return;
    if (key == Qt::Key_Up || key==Qt::Key_Left || key==Qt::Key_Right || key==Qt::Key_Down) 
    {
        switch (player2->getState())
        {
        case HS_move_up:
            player2->setState(HS_stand_up);
            break;
        case HS_move_left:
            player2->setState(HS_stand_left);
            break;
        case HS_move_down:
            player2->setState(HS_stand_down);
            break;
        case HS_move_right:
            player2->setState(HS_stand_right);
            break;
        }
        player2->velocity.setX(0);
        player2->velocity.setY(0);
    }
}

void Game::mouseReleaseEvent(QMouseEvent *event)
{
    if (!player1->haveFlash())
        return;

    QPoint mousePos = event->pos();
    QRect newCollider(mousePos.x() - 13, mousePos.y(), 26, 14);
    int newX = mousePos.x() - 13;
    int newY = mousePos.y() - 30;

    // out of the map
    if (mousePos.x() < MAP_BLOCK_LEFT || mousePos.x() > MAP_BLOCK_RIGHT || mousePos.y() < MAP_BLOCK_UP || mousePos.y() > MAP_BLOCK_DOWN)
        return;

    // at the edge of the map
    QRect mapLeftEdge(MAP_BLOCK_LEFT - 1, MAP_BLOCK_UP, 1, MAP_BLOCK_DOWN - MAP_BLOCK_UP);
    QRect mapRightEdge(MAP_BLOCK_RIGHT, MAP_BLOCK_UP, 1, MAP_BLOCK_DOWN - MAP_BLOCK_UP);
    QRect mapUpEdge(MAP_BLOCK_LEFT, MAP_BLOCK_UP - 1, MAP_BLOCK_RIGHT - MAP_BLOCK_LEFT, 1);
    QRect mapDownEdge(MAP_BLOCK_LEFT, MAP_BLOCK_DOWN, MAP_BLOCK_RIGHT - MAP_BLOCK_LEFT, 1);
    if (newCollider.intersects(mapLeftEdge))
    {
        newX = MAP_BLOCK_LEFT;
        if (newCollider.intersects(mapDownEdge))
            newY = MAP_BLOCK_DOWN - 44;
        if (newCollider.intersects(mapUpEdge))
            newY = MAP_BLOCK_UP - 30;
        player1->setPosition(newX, newY);
        return;
    }
    if (newCollider.intersects(mapRightEdge))
    {
        newX = MAP_BLOCK_RIGHT - 26;
        if (newCollider.intersects(mapDownEdge))
            newY = MAP_BLOCK_DOWN - 44;
        if (newCollider.intersects(mapUpEdge))
            newY = MAP_BLOCK_UP - 30;
        player1->setPosition(newX, newY);
        return;
    }
    if (newCollider.intersects(mapDownEdge))
    {
        newY = MAP_BLOCK_DOWN - 44;
        player1->setPosition(newX, newY);
        return;
    }
    if (newCollider.intersects(mapUpEdge))
    {
        newY = MAP_BLOCK_UP - 30;
        player1->setPosition(newX, newY);
        return;
    }

    // in the box matrix
    int r, c;
    int matrixLeft = MAP_BLOCK_LEFT + (MAP_BLOCK_RIGHT - MAP_BLOCK_LEFT - N * CUBE_LENGTH) / 2;
    int matrixRight = matrixLeft + N * CUBE_LENGTH;
    int matrixUp = MAP_BLOCK_UP + (MAP_BLOCK_DOWN - MAP_BLOCK_UP - M * CUBE_LENGTH) / 2;
    int matrixDown = matrixUp + M * CUBE_LENGTH;
    if (mousePos.x() > matrixLeft && mousePos.x() < matrixRight && mousePos.y() > matrixUp && mousePos.y() < matrixDown)
    {
        c = (mousePos.x() - matrixLeft) / CUBE_LENGTH + 1;
        r = (mousePos.y() - matrixUp) / CUBE_LENGTH + 1;
        // 边界上
        if (r == 1 || c == 1 || r == M || c == N)
        {
            // 空点
            if (boxMatrix[r][c] == 0)
            {
                newX = matrixLeft + (c - 0.5) * CUBE_LENGTH - 13;
                newY = matrixUp + (r - 0.5) * CUBE_LENGTH - 30;
            }
            // 非空点
            else
            {
                if (c == 1)
                    newX = matrixLeft - 0.5 * CUBE_LENGTH - 13;
                if (r == 1)
                    newY = matrixUp - 0.5 * CUBE_LENGTH - 30;
                if (c == N)
                    newX = matrixRight + 0.5 * CUBE_LENGTH - 13;
                if (r == M)
                    newY = matrixDown + 0.5 * CUBE_LENGTH - 30;
                triggerBox(r, c);
            }
            player1->setPosition(newX, newY);
            return;
        }
        // 内部
        else
        {
            // 空点
            if (boxMatrix[r][c] == 0)
            {
                if (canReachEdge(r, c))
                {
                    newX = matrixLeft + (c - 0.5) * CUBE_LENGTH - 13;
                    newY = matrixUp + (r - 0.5) * CUBE_LENGTH - 30;
                    player1->setPosition(newX, newY);
                }
                return;
                // 非空点
            }
            else
            {
                if (canReachEdge(r - 1, c))
                {
                    newX = matrixLeft + (c - 0.5) * CUBE_LENGTH - 13;
                    newY = matrixUp + (r - 1.5) * CUBE_LENGTH - 30;
                    player1->setPosition(newX, newY);
                    triggerBox(r, c);
                    return;
                }
                if (canReachEdge(r + 1, c))
                {
                    newX = matrixLeft + (c - 0.5) * CUBE_LENGTH - 13;
                    newY = matrixUp + (r + 0.5) * CUBE_LENGTH - 30;
                    player1->setPosition(newX, newY);
                    triggerBox(r, c);
                    return;
                }
                if (canReachEdge(r, c - 1))
                {
                    newX = matrixLeft + (c - 1.5) * CUBE_LENGTH - 13;
                    newY = matrixUp + (r - 0.5) * CUBE_LENGTH - 30;
                    player1->setPosition(newX, newY);
                    triggerBox(r, c);
                    return;
                }
                if (canReachEdge(r, c + 1))
                {
                    newX = matrixLeft + (c + 0.5) * CUBE_LENGTH - 13;
                    newY = matrixUp + (r - 0.5) * CUBE_LENGTH - 30;
                    player1->setPosition(newX, newY);
                    triggerBox(r, c + 1);
                    return;
                }
                return;
            }
        }
    }

    // at the edge of the box matrix
    QRect leftEdge(matrixLeft - 1, matrixUp, 1, matrixDown - matrixUp);
    QRect rightEdge(matrixRight, matrixUp, 1, matrixDown - matrixUp);
    QRect upEdge(matrixLeft, matrixUp - 1, matrixRight - matrixLeft, 1);
    QRect downEdge(matrixLeft, matrixDown, matrixRight - matrixLeft, 1);
    if (newCollider.intersects(leftEdge))
    {
        newX = matrixLeft - 27;
        if (newCollider.intersects(mapDownEdge))
            newY = matrixDown - 29;
        if (newCollider.intersects(mapUpEdge))
            newY = MAP_BLOCK_UP - 45;
        player1->setPosition(newX, newY);
        return;
    }
    if (newCollider.intersects(rightEdge))
    {
        newX = matrixRight + 1;
        if (newCollider.intersects(mapDownEdge))
            newY = matrixDown - 29;
        if (newCollider.intersects(mapUpEdge))
            newY = matrixUp - 45;
        player1->setPosition(newX, newY);
        return;
    }
    if (newCollider.intersects(downEdge))
    {
        newY = matrixDown - 29;
        player1->setPosition(newX, newY);
        return;
    }
    if (newCollider.intersects(mapUpEdge))
    {
        newY = MAP_BLOCK_UP - 45;
        player1->setPosition(newX, newY);
        return;
    }

    player1->setPosition(newX, newY);
}

// 功能函数
void Game::generateBox(bool generateMatrix)
{
    // generate Box matrix
    if (generateMatrix == true)
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
            if (boxMatrix[i + 1][j + 1] == 0)
                continue;
            x = startX + j * CUBE_LENGTH;
            Box *box = new Box(x, y, (BoxColor)boxMatrix[i + 1][j + 1]);
            box->setMatrixPosition(i + 1, j + 1);
            adjustBox(box);
            Mgr->addEntity(box);
        }
    }
}

void Game::generateItem()
{
    if(itemGenerateElapsedTimer.elapsed()<QRandomGenerator::global()->bounded(10, 20)*1000)
        return;
    itemGenerateElapsedTimer.restart();

    Item *item;

    // random the type
    int itemType = QRandomGenerator::global()->bounded(ITEM_TYPE_NUM);
    if (state == GS_double_mode) // delete flash in the double mode
        while (itemType == IT_flash)
            itemType = QRandomGenerator::global()->bounded(ITEM_TYPE_NUM);
    if (state == GS_single_mode)
        while (itemType == IT_dizzy || itemType==IT_freeze)
            itemType = QRandomGenerator::global()->bounded(ITEM_TYPE_NUM);

    item = new Item((ItemType)itemType);

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

}

void Game::boxCollitionDect1()
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
    if (triggerElapsedTimer1.elapsed() < 600)
        return;
    triggerElapsedTimer1.restart();

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

void Game::boxCollitionDect2()
{
    QSet<Box *> triggeredBoxes;
    // dect all the collision
    for (auto i : Mgr->getEntity(ET_box))
    {
        Box *foo = (Box *)i;
        if (player2->intersects(foo->getCollider()))
        {
            player2->collideEvent();
            triggeredBoxes.insert(foo);
        }
    }

    // if only one collision && trigger elapse>500 , then triggered
    if (triggeredBoxes.size() != 1)
        return;
    if (triggerElapsedTimer2.elapsed() < 600)
        return;
    triggerElapsedTimer2.restart();

    // boxes' trigger event
    Box *foo = *triggeredBoxes.begin();
    foo->trigger();

    // player's trigger event
    if (player2->getTriggeredBox() == nullptr)
    {
        player2->addTriggeredBox(foo);
        qInfo() << "player2 get:" << player2->getTriggeredBox();
    }
    else
    {
        qInfo() << "player2 abandon:" << player2->getTriggeredBox();
        ElimateBox(player2->getTriggeredBox(), foo, 2);
        player2->resetTriggeredBox();
    }
}

void Game::itemCollitionDect()
{
    for (auto i : Mgr->getEntity(ET_item))
    {
        Item *item = (Item *)i;

        // player 1
        if (player1->intersects(item->getCollider()))
        {
            switch (item->getItemType())
            {
            case IT_add1s:
                progressBar->addTime(30);
                break;
            case IT_flash:
                player1->addBuff(BT_flash);
                break;
            case IT_shuffle:
                shuffleBox();
                break;
            case IT_dizzy:
                player2->addBuff(BT_dizzy);
                break;
            case IT_freeze:
                player2->addBuff(BT_freeze);
            }
            item->pickUp();
        }

        // player 2
        if (player2->intersects(item->getCollider()))
        {
            switch (item->getItemType())
            {
            case IT_add1s:
                progressBar->addTime(30);
                break;
            case IT_flash:
                player2->addBuff(BT_flash);
                break;
            case IT_shuffle:
                shuffleBox();
                break;
            case IT_dizzy:
                player1->addBuff(BT_dizzy);
                break;
            case IT_freeze:
                player1->addBuff(BT_freeze);
            }
            item->pickUp();
        }
    }
}

void Game::ElimateBox(Box *playerBox, Box *box, int playerID)
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

        if (elimatable(playerBox, box, playerID))
        {
            qInfo() << "elimate Box:" << playerBox;
            qInfo() << "elimate Box:" << box;
            connect(box, &Box::signalElimate, playerBox, &Box::elimate);
            connect(box, &Box::signalElimate, box, &Box::elimate);
            boxMatrix[playerBox->getR()][playerBox->getC()] = 0;
            boxMatrix[box->getR()][box->getC()] = 0;

            // score
            score(2, playerID);
        }
        else
        {
            connect(box, &Box::signalCancelTrigger, playerBox, &Box::cancelTrigger);
            connect(box, &Box::signalElimate, box, &Box::cancelTrigger);
        }
    }
}

void Game::score(int x, int playerID)
{
    if (playerID == 1)
        emit signalScore1(x);
    if (playerID == 2)
        emit signalScore2(x);
}

void Game::adjustBox(Box* box)
{
    int startX = MAP_BLOCK_LEFT + (MAP_BLOCK_RIGHT - MAP_BLOCK_LEFT - N * CUBE_LENGTH) / 2;
    int startY = MAP_BLOCK_UP + (MAP_BLOCK_DOWN - MAP_BLOCK_UP - M * CUBE_LENGTH) / 2;

    int r = box->getR();
    int c = box->getC();
    int x = startX + (c-1)*CUBE_LENGTH;
    int y = startY + (r-1)*CUBE_LENGTH;
    QRect collider(x, y, CUBE_LENGTH, CUBE_LENGTH);

    if(!player1->intersects(collider) && !player2->intersects(collider)) return;

    int color = box->getColor();
    boxMatrix[r][c] = 0;
    for(int i=1; i<=M; ++i)
    {
        for(int j=1; j<=N; ++j)
        {
            if(i==r && j==c) continue;
            if(boxMatrix[i][j]==0)
            {
                boxMatrix[i][j]==color;
                box->setMatrixPosition(i, j);
                box->setPosition(startX+(j-1)*CUBE_LENGTH, startY+(i-1)*CUBE_LENGTH);
                adjustBox(box);
                return;
            }
        }
    }
}

void Game::triggerBox(int r, int c)
{
    for (auto i : Mgr->getEntity(ET_box))
    {
        Box *box = (Box *)i;
        if (box->getR() == r && box->getC() == c)
        {
            // boxes' trigger event
            box->trigger();

            // player's trigger event
            if (player1->getTriggeredBox() == nullptr)
            {
                player1->addTriggeredBox(box);
                qInfo() << "player1 get:" << player1->getTriggeredBox();
            }
            else
            {
                qInfo() << "player1 abandon:" << player1->getTriggeredBox();
                ElimateBox(player1->getTriggeredBox(), box);
                player1->resetTriggeredBox();
            }
            return;
        }
    }
}

void Game::shuffleBox()
{
    // reset the box
    BoxColor color1 = BC_none;
    BoxColor color2 = BC_none;
    if (player1->getTriggeredBox() != nullptr)
        color1 = player1->getTriggeredBox()->getColor();
    if (player2->getTriggeredBox() != nullptr)
        color2 = player2->getTriggeredBox()->getColor();

    Mgr->cleanEntityType(ET_box);

    // flaten
    int flatArray[MAX_M * MAX_N];
    int index = 0;
    for (int i = 1; i <= M; ++i)
        for (int j = 1; j <= N; ++j)
            flatArray[index++] = boxMatrix[i][j];

    // shuffle
    for (int i = M * N - 1; i > 0; --i)
    {
        int j = QRandomGenerator::global()->bounded(i + 1); // Generate a random index
        std::swap(flatArray[i], flatArray[j]);              // Swap elements
    }

    // back to the boxMatrix
    index = 0;
    for (int i = 1; i <= M; ++i)
        for (int j = 1; j <= N; ++j)
            boxMatrix[i][j] = flatArray[index++];

    // generateBox
    generateBox(false);

    // triggerBox
    if (color1 != BC_none)
    {
        for (auto i : Mgr->getEntity(ET_box))
        {
            Box *box = (Box *)i;
            if (box->getColor() == color1)
            {
                player1->addTriggeredBox(box);
                box->trigger();
                break;
            }
        }
    }

    if (color2 != BC_none)
    {
        for (auto i : Mgr->getEntity(ET_box))
        {
            Box *box = (Box *)i;
            if (box->getColor() == color2)
            {
                player2->addTriggeredBox(box);
                box->trigger();
                break;
            }
        }
    }
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

void Game::addLine(int r1, int c1, int r2, int c2, int playerID)
{
    int startX = MAP_BLOCK_LEFT + (MAP_BLOCK_RIGHT - MAP_BLOCK_LEFT - N * CUBE_LENGTH) / 2;
    int startY = MAP_BLOCK_UP + (MAP_BLOCK_DOWN - MAP_BLOCK_UP - M * CUBE_LENGTH) / 2;
    int x1 = startX + (c1 - 0.5) * CUBE_LENGTH;
    int y1 = startY + (r1 - 0.5) * CUBE_LENGTH;
    int x2 = startX + (c2 - 0.5) * CUBE_LENGTH;
    int y2 = startY + (r2 - 0.5) * CUBE_LENGTH;

    switch (playerID)
    {
    case 1:
        lineSet1.insert(new QLine(x1, y1, x2, y2));
        showPathElapsedTimer1.restart();
        break;
    case 2:
        lineSet2.insert(new QLine(x1, y1, x2, y2));
        showPathElapsedTimer2.restart();
        break;
    }
}

void Game::drawPath1(QPainter *painter)
{
    if (lineSet1.isEmpty())
        return;

    QPen pen;
    pen.setColor(QColor(173, 216, 230, 256 * 0.8));
    pen.setWidth(8);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);

    for (auto i : lineSet1)
        painter->drawLine(*i);

    if (showPathElapsedTimer1.elapsed() > 500)
        for (auto i : lineSet1)
        {
            lineSet1.remove(i);
            delete i;
        }
}

void Game::drawPath2(QPainter *painter)
{
    if (lineSet2.isEmpty())
        return;

    QPen pen;
    pen.setColor(QColor(255, 255, 204, 256 * 0.8));
    pen.setWidth(8);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);

    for (auto i : lineSet2)
        painter->drawLine(*i);

    if (showPathElapsedTimer2.elapsed() > 500)
        for (auto i : lineSet2)
        {
            lineSet2.remove(i);
            delete i;
        }
}

void Game::takeScreenShoot()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    screenshot = screen->grabWindow(this->winId());
}

void Game::drawShadow(QPainter *painter)
{
    painter->drawPixmap(0, 0, screenshot);

    QRect rect(0, 0, MAP_WIDTH * CUBE_LENGTH, MAP_HEIGHT * CUBE_LENGTH);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0, 0, 0, 100));
    painter->drawRect(rect);
}

void Game::saveGameState(const QString &filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "无法打开文件:" << file.errorString();
        return;
    }

    QString fileContent;
    QTextStream in(&file);
    while (!in.atEnd())
        fileContent += in.readLine() + "\n";
    file.close();

    QRegularExpression regex("gamemode:\\s*\\S+");
    if (gameMode == GS_single_mode)
        fileContent.replace(regex, "gamemode: singleMode");
    if (gameMode == GS_double_mode)
        fileContent.replace(regex, "gamemode: doubleMode");

    // 重新打开文件以写入更新后的内容
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "无法打开文件:" << file.errorString();
        return;
    }

    QTextStream out(&file);
    out << fileContent; // 写入修改后的内容
    file.close();       // 关闭文件
}

void Game::loadGameState(const QString &filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "无法打开文件:" << file.errorString();
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        if (line.startsWith("gamemode:"))
        {
            QString mode = line.section(QRegularExpression("\\s"), 1, 1).trimmed();
            if (mode == "doubleMode")
                gameMode = GS_double_mode;
            if (mode == "singleMode")
                gameMode = GS_single_mode;
            break;
        }
    }

    file.close();
}

void Game::saveBoxes(const QString &filePath)
{
    QFile file(filePath);

    // 读取文件内容
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "无法打开文件:" << file.errorString();
        return;
    }

    QString beforeMapContent; // 保存 map: 之前的内容
    QString mapPartContent;   // 保存 map: 和 map 之后的内容
    bool foundMap = false;

    QTextStream in(&file);

    // 逐行读取文件
    while (!in.atEnd())
    {
        QString line = in.readLine();
        if (line.startsWith("map:"))
        {
            foundMap = true;
            beforeMapContent += line + "\n"; // 保存 map: 这一行
            continue;
        }

        if (foundMap)
            mapPartContent += line + "\n"; // 保留 map 后的其它内容
        else
            beforeMapContent += line + "\n"; // 保存 map: 之前的内容
    }
    file.close();

    // 更新 M 和 N 的值
    QRegularExpression regexM("M:\\s*\\d*");
    QRegularExpression regexN("N:\\s*\\d*");
    beforeMapContent.replace(regexM, QString("M: %1").arg(M));
    beforeMapContent.replace(regexN, QString("N: %1").arg(N));

    // 生成 map 的数据内容
    QString mapData;
    for (int i = 1; i <= M; ++i)
    {
        for (int j = 1; j <= N; ++j)
        {
            mapData += QString::number(boxMatrix[i][j]) + " ";
        }
        mapData += "\n"; // 每行结束换行
    }

    // 以写入模式重新打开文件
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "无法写入文件:" << file.errorString();
        return;
    }

    QTextStream out(&file);
    out << beforeMapContent; // 写入 map: 之前的内容
    out << mapData;          // 写入更新后的 map 数据
    out << mapPartContent;   // 写入 map: 之后的内容
    file.close();

    qDebug() << "# Map 更新成功!";
}

void Game::loadBoxes(const QString &filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "无法打开文件:" << file.errorString();
        return;
    }

    QTextStream in(&file);
    QString line;

    // 读取 M 和 N
    while (!in.atEnd())
    {
        line = in.readLine().trimmed();
        if (line.startsWith("M:"))
        {
            M = line.section(':', 1, 1).toInt();
        }
        else if (line.startsWith("N:"))
        {
            N = line.section(':', 1, 1).toInt();
        }
        else if (line == "map:")
        {
            break;
        }
    }

    // 清除原矩阵
    for (int i = 0; i < MAX_M; ++i)
        for (int j = 0; j < MAX_M; ++j)
            boxMatrix[i][j] = 0;

    // 读取 map 矩阵
    for (int i = 1; i <= M && !in.atEnd(); ++i)
    {
        line = in.readLine().trimmed();
        QStringList numbers = line.split(QRegularExpression("\\s+"));
        for (int j = 1; j <= N && j <= numbers.size(); ++j)
        {
            boxMatrix[i][j] = numbers[j - 1].toInt();
        }
    }

    // generate Box
    generateBox(false);

    file.close();
    qInfo() << "load Boxes Successfully !";
}

void Game::saveItems(const QString &filePath)
{
    QFile file(filePath);
    int num;

    // 读取文件内容
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "无法打开文件:" << file.errorString();
        return;
    }

    // item
    QString beforeItemContent; // 保存 item.num 和 item.num: 之前的内容
    QString itemPartContent;   // 保存 item.num 之后的内容
    bool foundItem = false;
    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();
        if (line.startsWith("item.num:"))
        {
            foundItem = true;
            beforeItemContent += line + "\n"; // 保存 item.num: 这一行
            continue;
        }

        if (foundItem)
            itemPartContent += line + "\n"; // 保留 item.num 后的其它内容
        else
            beforeItemContent += line + "\n"; // 保存 item.num 之前的内容
    }
    file.close();

    // 更新 item.num
    num = Mgr->getEntity(ET_item).count();
    QRegularExpression regex("item\\.num:\\s*\\d*");
    beforeItemContent.replace(regex, QString("item.num: %1").arg(num));

    // 生成 item
    QString itemData;
    Item *item;
    for (auto i : Mgr->getEntity(ET_item))
    {
        item = (Item *)i;
        itemData += QString::number(item->getItemType()) + " " + QString::number(item->getX()) + " " + QString::number(item->getY()) + "\n";
    }

    // 以写入模式重新打开文件
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "无法写入文件:" << file.errorString();
        return;
    }

    QTextStream out(&file);
    out << beforeItemContent << itemData << itemPartContent;
    file.close();

    qDebug() << "Item 更新成功!";
}

void Game::loadItems(const QString &filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "无法打开文件:" << file.errorString();
        return;
    }

    QTextStream in(&file);
    QString line;
    int num = 0;

    // 读取 item.num
    while (!in.atEnd())
    {
        line = in.readLine().trimmed();
        if (line.startsWith("item.num:"))
        {
            num = line.section(':', 1, 1).toInt();
            break;
        }
    }

    // 读取 item
    for (int i = 0; i < num && !in.atEnd(); ++i)
    {
        line = in.readLine().trimmed();
        QStringList numbers = line.split(QRegularExpression("\\s+"));
        Mgr->addEntity(new Item((ItemType)numbers[0].toInt(), numbers[1].toInt(), numbers[2].toInt()));
    }

    file.close();
    qInfo() << "load Boxes Successfully !";
}

void Game::setBoxMatrix(int matrix[MAX_M+2][MAX_N+2], int m, int n)
{
    if(M>MAX_M || N>MAX_N) qDebug() << "M*N out of range!";
    M = m, N =n;
    for(int i=0; i<=M+1; ++i)
        for(int j=0; j<=N+1; ++j)
            boxMatrix[i][j] = matrix[i][j];
}

// 判定函数
bool Game::elimatable(int r1, int c1, int r2, int c2, int showPath)
{
    if(boxMatrix[r1][c1]!=boxMatrix[r2][c2] || boxMatrix[r1][c1]==0 || boxMatrix[r2][c2]==0) return false;
    return horizonElimatable(r1, c1, r2, c2, showPath) || verticalElimatable(r1, c1, r2, c2, showPath) || oneCornerElimatable(r1, c1, r2, c2, showPath) || twoCornerElimatable(r1, c1, r2, c2, showPath);
}

bool Game::elimatable(const Box *box1, const Box *box2, int showPath)
{   
    int r1 = box1->getR();
    int c1 = box1->getC();
    int r2 = box2->getR();
    int c2 = box2->getC();

    return elimatable(r1, c1, r2, c2, showPath);
}

bool Game::horizonElimatable(int r1, int c1, int r2, int c2, int showPath)
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
    if (showPath)
        addLine(r1, c1, r2, c2, showPath);

    return true;
}

bool Game::horizonElimatable(const Box *box1, const Box *box2, int showPath)
{
    int r1 = box1->getR();
    int c1 = box1->getC();
    int r2 = box2->getR();
    int c2 = box2->getC();

    return horizonElimatable(r1, c1, r2, c2, showPath);
}

bool Game::verticalElimatable(int r1, int c1, int r2, int c2, int showPath)
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
    if (showPath)
        addLine(r1, c1, r2, c2, showPath);

    return true;
}

bool Game::verticalElimatable(const Box *box1, const Box *box2, int showPath)
{
    int r1 = box1->getR();
    int c1 = box1->getC();
    int r2 = box2->getR();
    int c2 = box2->getC();

    return verticalElimatable(r1, c1, r2, c2, showPath);
}

bool Game::oneCornerElimatable(int r1, int c1, int r2, int c2, int showPath)
{
    // find the corner in the same r with box1
    // 2     2
    // |--1--|
    // 2     2
    for (int i = c1 + 1; i <= N && boxMatrix[r1][i] == 0; ++i)
        if (verticalElimatable(r1, i, r2, c2))
        {
            addLine(r1, i, r2, c2, showPath);
            addLine(r1, c1, r1, i, showPath);
            return true;
        }
    for (int i = c1 - 1; i >= 1 && boxMatrix[r1][i] == 0; --i)
        if (verticalElimatable(r1, i, r2, c2))
        {
            addLine(r1, i, r2, c2, showPath);
            addLine(r1, c1, r1, i, showPath);
            return true;
        }
    // find the corner in the same c with box1
    //  2---2
    //    1
    //  2---2
    for (int i = r1 + 1; i <= M && boxMatrix[i][c1] == 0; ++i)
        if (horizonElimatable(i, c1, r2, c2))
        {
            addLine(i, c1, r2, c2, showPath);
            addLine(r1, c1, i, c1, showPath);
            return true;
        }
    for (int i = r1 - 1; i >= 1 && boxMatrix[i][c1] == 0; --i)
        if (horizonElimatable(i, c1, r2, c2))
        {
            addLine(i, c1, r2, c2, showPath);
            addLine(r1, c1, i, c1, showPath);
            return true;
        }
    return false;
}

bool Game::oneCornerElimatable(const Box *box1, const Box *box2, int showPath)
{
    int r1 = box1->getR();
    int c1 = box1->getC();
    int r2 = box2->getR();
    int c2 = box2->getC();

    return oneCornerElimatable(r1, c1, r2, c2, showPath);
}

bool Game::twoCornerElimatable(int r1, int c1, int r2, int c2, int showPath)
{
    // find the corner in same r with box1
    //  2-.-2
    //    |
    // 1--.
    //    |
    //  2-.-2
    for (int i = c1 + 1; i <= N + 1 && boxMatrix[r1][i] == 0; ++i)
    {
        // corner1(r1, i), corner2(r2, i)
        if (boxMatrix[r2][i] != 0)
            continue;
        if (verticalElimatable(r1, i, r2, i) && horizonElimatable(r2, i, r2, c2))
        {
            addLine(r1, c1, r1, i, showPath);
            addLine(r1, i, r2, i, showPath);
            addLine(r2, i, r2, c2, showPath);
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
        if (boxMatrix[r2][i] != 0)
            continue;
        if (verticalElimatable(r1, i, r2, i) && horizonElimatable(r2, i, r2, c2))
        {
            addLine(r1, c1, r1, i, showPath);
            addLine(r1, i, r2, i, showPath);
            addLine(r2, i, r2, c2, showPath);
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
        if (boxMatrix[i][c2] != 0)
            continue;
        if (horizonElimatable(i, c1, i, c2) && verticalElimatable(i, c2, r2, c2))
        {
            addLine(r1, c1, i, c1, showPath);
            addLine(i, c1, i, c2, showPath);
            addLine(i, c2, r2, c2, showPath);
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
        if (boxMatrix[i][c2] != 0)
            continue;
        ;
        if (horizonElimatable(i, c1, i, c2) && verticalElimatable(i, c2, r2, c2))
        {
            addLine(r1, c1, i, c1, showPath);
            addLine(i, c1, i, c2, showPath);
            addLine(i, c2, r2, c2, showPath);
            return true;
        }
    }

    return false;
}

bool Game::twoCornerElimatable(const Box *box1, const Box *box2, int showPath)
{
    int r1 = box1->getR();
    int c1 = box1->getC();
    int r2 = box2->getR();
    int c2 = box2->getC();

    return twoCornerElimatable(r1, c1, r2, c2, showPath);
}

bool Game::canReachEdge(const int r, const int c)
{
    if (boxMatrix[r][c] != 0)
        return false;

    std::queue<std::pair<int, int>> q;
    bool visited[MAX_M][MAX_N] = {};
    const std::pair<int, int> directions[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    q.push({r, c});
    visited[r][c] = true;

    while (!q.empty())
    {
        auto [r, c] = q.front();
        q.pop();

        // point on edge
        if (r == 1 || r == M || c == 1 || c == N)
            return true;

        for (const auto &direction : directions)
        {
            int newR = r + direction.first;
            int newC = c + direction.second;
            if (r >= 1 && r <= M && c >= 1 && c <= N && !visited[newR][newC] && boxMatrix[newR][newC] == 0)
            {
                q.push({newR, newC});
                visited[newR][newC] = true;
            }
        }
    }

    return false;
}
