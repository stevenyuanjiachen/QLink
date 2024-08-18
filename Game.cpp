#include "Game.h"
#include "Hero.h"
#include "Manager.h"

Hero *player1;

Game::Game(QWidget *parent)
    : QWidget(parent)
{
    QTimer *timer = new QTimer(this);
    timer->callOnTimeout(this, [=]()
                         {
        update_game();
        update(); });
    timer->start(1000 / 60);
}

void Game::run()
{
    init_game(560, 450, "QLink");
    this->show();
}

void Game::init_game(
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

    player1 = new Hero(0, 0);
    Mgr->addEntity(player1);
}

void Game::draw_game(QPainter *painter)
{
    Mgr->draw(painter);
}

void Game::update_game()
{
    Mgr->update();
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
