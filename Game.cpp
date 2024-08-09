#include "Game.h"

// add Entities
Sprite* player;
Map* gameMap;

Game::Game(QWidget* parent)
    : QWidget(parent)
{
    QTimer* timer = new QTimer(this);
    timer->callOnTimeout(this, [=](){
        update_game();
        update();
    });
    timer->start(1000/60);
}

void Game::run()
{
    init_game(480, 700, "PlaneGame");
    this->show();
}

void Game::init_game(
    int w, int h, 
    const QString& title,
    const QIcon& icon)
{
    setFixedSize(w, h);
    setWindowTitle(title);
    if(!icon.isNull())
    {
        setWindowIcon(icon);
    }

    // add the entities
    gameMap = new Map(w, h);
    player = new Sprite(190, 400, "../images/me1.png");
    mgr.addEntity(player);
}

void Game::draw_game(QPainter *painter)
{
    gameMap->draw(painter);
    mgr.draw(painter);
}

void Game::update_game()
{
    gameMap->update();
    mgr.update();
    
    // 限制飞机范围
    if(player->position.x()<0) 
        player->position.setX(0);
    if(player->position.x()>gameMap->width()-player->width()) 
        player->position.setX(gameMap->width()-player->width()); 
    if(player->position.y()<0)  
        player->position.setY(0);
    if(player->position.y()>gameMap->height()-player->height()) 
        player->position.setY(gameMap->height()-player->height());
}

void Game::clean_game()
{

}

void Game::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    draw_game(&painter); 
}

void Game::closeEvent(QCloseEvent *event)
{
    this->clean_game();
}

void Game::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_W:
        player->velocity.setY(-1);
        break;
    case Qt::Key_A:
        player->velocity.setX(-1); 
        break;
    case Qt::Key_S:
        player->velocity.setY(1);
        break;
    case Qt::Key_D:
        player->velocity.setX(1);
        break;
    }
}

void Game::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_W:
        player->velocity.setY(0);
        break;
    case Qt::Key_A:
        player->velocity.setX(0);
        break;
    case Qt::Key_S:
        player->velocity.setY(0);
        break;
    case Qt::Key_D:
        player->velocity.setX(0);
        break;
    }
}
