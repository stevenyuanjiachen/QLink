#include "Game.h"

Sprite* player;

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
    init_game(560, 450, "PlaneGame");
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

    player = new Sprite(0, 0, "../images/enemy1.png");
}

void Game::draw_game(QPainter *painter)
{
    mgr.draw(painter);
}

void Game::update_game()
{
    mgr.update();
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
