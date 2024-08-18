#include "Game.h"
#include "Hero.h"
#include "Manager.h"

Hero* player1 = new Hero(0,0);
Mgr->addEntity(player1);

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
    init_game(560, 450, "QLink");
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
}

void Game::draw_game(QPainter *painter)
{

}

void Game::update_game()
{
    
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
    
        break;
    case Qt::Key_A:
        
        break;
    case Qt::Key_S:
        
        break;
    case Qt::Key_D:
        
        break;
    }
}

void Game::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_W:
        
        break;
    case Qt::Key_A:
        
        break;
    case Qt::Key_S:
        
        break;
    case Qt::Key_D:
        
        break;
    }
}
