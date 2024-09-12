#include "Game.h"
#include "Hero.h"
#include "Manager.h"
#include "Item.h"
#include "Map.h"
#include "MyProgressBar.h"
#include "ScoreBoard.h"
#include "PauseMenu.h"
#include "StartMenu.h"
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

// the Box Matrix is M*N
int M = 8, N = 8;

Hero *player1;
Hero *player2;
Map *gameMap;
MyProgressBar *progressBar;
ScoreBoard *scoreBoard1;
ScoreBoard *scoreBoard2;
PauseMenu *pauseMenu;
StartMenu *startMenu;

// 游戏实现
Game::Game(QWidget *parent)
    : QWidget(parent), state(GS_start)
{
    // game timer
    QTimer *timer = new QTimer(this);
    timer->callOnTimeout(this, [=](){
        updateGame();
        update(); });
    timer->start(1000 / GAME_FPS);

    itemGenerateTimer.callOnTimeout(this, &Game::generateItem);
    itemGenerateTimer.start(5000);

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
    if (!icon.isNull())    // icon
    {
        setWindowIcon(icon);
    }

    // generate map
    gameMap = new Map();

    // generate player
    player1 = new Hero(MAP_BLOCK_LEFT + 10, MAP_BLOCK_UP + 10);
    player2 = new Hero(MAP_BLOCK_RIGHT - player1->pixmap.width() - 10,
                        MAP_BLOCK_DOWN - player1->pixmap.height() - 10, 2);
    Mgr->addEntity(player1);
    triggerElapsedTimer1.start();
    triggerElapsedTimer2.start();

    // generate Box matric
    generateBox();

    // generate the Progress Bar
    progressBar = new MyProgressBar((CUBE_LENGTH * MAP_WIDTH - MY_PROGRESS_BAR_WIDTH) / 2, CUBE_LENGTH, 60);
    connect(progressBar, &MyProgressBar::signalEnd, this, &Game::finishGame);
    connect(this, &Game::signalPause, progressBar, &MyProgressBar::pauseBar);
    connect(this, &Game::signalContinue, progressBar, &MyProgressBar::contineBar);

    // generate the scoreBoard
    scoreBoard1 = new ScoreBoard(0, 0);
    scoreBoard2 = new ScoreBoard(0, 50);
    connect(this, &Game::signalScore1, scoreBoard1, &ScoreBoard::addScore);
    connect(this, &Game::signalScore2, scoreBoard2, &ScoreBoard::addScore);

    // generate the puase menu
    pauseMenu = new PauseMenu(this, (MAP_WIDTH * CUBE_LENGTH - PAUSE_MENU_WIDTH) / 2, (MAP_HEIGHT * CUBE_LENGTH - PAUSE_MENU_HEIGHT) / 2);
    connect(pauseMenu, &PauseMenu::signalContinue, this, &Game::continueGame);
    connect(pauseMenu, &PauseMenu::signalSaveGame, this, &Game::saveGame);
    connect(pauseMenu, &PauseMenu::signalLoadGame, this, &Game::loadGame);

    // generate the start menu
    startMenu = new StartMenu(this);
    connect(startMenu, &StartMenu::signalSingleMode, this, [=](){
        state = GS_single_mode;
        startMenu->hide(); });
    connect(startMenu, &StartMenu::signalDoubleMode, this, [=](){
        Mgr->addEntity(player2);
        state = GS_double_mode;
        startMenu->hide(); });
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
        emit signalPause();
        itemGenerateTimer.start(5000);
        break;
    case GS_single_mode:
        Mgr->update();
        boxCollitionDect1();
        itemCollitionDect();
        //        solubleCheck();
        break;
    case GS_double_mode:
        Mgr->update();
        boxCollitionDect1();
        boxCollitionDect2();
        itemCollitionDect();
        //        solubleCheck();
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

void Game::pauseGame()
{
    QScreen* screen = QGuiApplication::primaryScreen();
    screenshot = screen->grabWindow(this->winId());
    state = GS_pause;
}

void Game::continueGame()
{
    emit signalContinue();
    state = GS_single_mode;
    pauseMenu->hide();
}

void Game::saveGame()
{
    const QString filePath = "../saves/test.txt";

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
    {
        qDebug() << "文件复制成功!";
    }
    else
    {
        qDebug() << "文件复制失败!";
    }

    saveBoxes(filePath);              // save box
    player1->saveHeroState(filePath); // save hero
    saveItems(filePath);              // save item
    progressBar->saveState(filePath); // save progressBar
    scoreBoard1->saveState(filePath); // save scoreBoard
}

void Game::loadGame()
{
    const QString &filePath = "../saves/test.txt";

    Mgr->clean();        // clean Game
    loadBoxes(filePath); // load boxes
    player1 = new Hero(0, 0);
    player1->loadHeroState(filePath);
    Mgr->addEntity(player1);          // load player
    loadItems(filePath);              // load items
    progressBar->loadState(filePath); // load progressBar
    scoreBoard1->loadState(filePath); // load scoreBoard1

    continueGame();
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
    if (state == GS_pause)
        return;

    // player 1
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

    // player 2
    if(this->state != GS_double_mode) return;
    if (player2->getState() == HS_stand_down ||
        player2->getState() == HS_stand_up ||
        player2->getState() == HS_stand_left ||
        player2->getState() == HS_stand_right)
    {
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
}

void Game::keyReleaseEvent(QKeyEvent *event)
{   
    // pause
    if (event->key() == Qt::Key_Escape)
    {
        pauseGame();
    }

    // player 1
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

    // player2
    if(this->state != GS_double_mode) return;
    if (player2->getState() == HS_move_down ||
        player2->getState() == HS_move_up ||
        player2->getState() == HS_move_left ||
        player2->getState() == HS_move_right)
    {
        switch (event->key())
        {
        case Qt::Key_Up:
            player2->setState(HS_stand_up);
            player2->velocity.setX(0);
            player2->velocity.setY(0);
            break;
        case Qt::Key_Left:
            player2->setState(HS_stand_left);
            player2->velocity.setX(0);
            player2->velocity.setY(0);
            break;
        case Qt::Key_Down:
            player2->setState(HS_stand_down);
            player2->velocity.setX(0);
            player2->velocity.setY(0);
            break;
        case Qt::Key_Right:
            player2->setState(HS_stand_right);
            player2->velocity.setX(0);
            player2->velocity.setY(0);
            break;
        }
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
            Mgr->addEntity(box);
        }
    }
}

void Game::generateItem()
{
    Item *item;

    // random the type
    int itemType = QRandomGenerator::global()->bounded(ITEM_TYPE_NUM);
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

    // random the time
    itemGenerateTimer.setInterval(1000 * QRandomGenerator::global()->bounded(3, 8));
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
    if (triggerElapsedTimer1.elapsed() < 500)
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
    if (triggerElapsedTimer2.elapsed() < 500)
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
                progressBar->addTime(1);
                break;
            case IT_flash:
                player1->addBuff(BT_flash);
                break;
            }
            item->pickUp();
        }

        // player 2
        if (player2->intersects(item->getCollider()))
        {
            switch (item->getItemType())
            {
            case IT_add1s:
                progressBar->addTime(1);
                break;
            case IT_flash:
                player2->addBuff(BT_flash);
                break;
            }
            item->pickUp();
        }
    }
}

void Game::ElimateBox(Box* playerBox, Box *box, int playerID)
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
    if(playerID == 1)
        emit signalScore1(x);
    if(playerID == 2)
        emit signalScore2(x);
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

void Game::drawPauseMenu(QPainter *painter)
{
    painter->drawPixmap(0, 0, screenshot);
    
    QRect rect(0, 0, MAP_WIDTH * CUBE_LENGTH, MAP_HEIGHT * CUBE_LENGTH);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0, 0, 0, 100));
    painter->drawRect(rect);
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

// 判定函数
bool Game::elimatable(const Box *box1, const Box *box2, int showPath)
{
    return horizonElimatable(box1, box2, showPath) || verticalElimatable(box1, box2, showPath) || oneCornerElimatable(box1, box2, showPath) || twoCornerElimatable(box1, box2, showPath);
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

bool Game::oneCornerElimatable(const Box *box1, const Box *box2, int showPath)
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

bool Game::twoCornerElimatable(const Box *box1, const Box *box2, int showPath)
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
